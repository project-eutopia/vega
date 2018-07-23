#include "vega/dictionary/dictionary.h"

#include "vega/multi_vr.h"
#include "vega/vr.h"
#include "vega/vm.h"

#include <fstream>
#include <string>
#include <regex>
#include <algorithm>

namespace vega {
  namespace dictionary {
    const std::string& Dictionary::default_dictionary_file_name() {
      static const std::string filename = "/usr/local/share/vega/dictionary.txt";
      return filename;
    }

    void set_dictionary(const std::string& file_name) {
      Dictionary::set_dictionary(file_name);
    }

    const Dictionary& instance(bool allow_default) {
      return Dictionary::instance(allow_default);
    }

    std::unique_ptr<Dictionary> Dictionary::singleton_ = nullptr;

    void Dictionary::set_dictionary(const std::string& file_name) {
      Dictionary::singleton_ = std::unique_ptr<Dictionary>(new Dictionary(file_name));
    }

    const Dictionary& Dictionary::instance(bool allow_default) {
      if (!Dictionary::singleton_) {
        // If allowing default dictionary, then can initialize it now
        if (allow_default) {
          set_dictionary(default_dictionary_file_name());
        }

        // If still no dictionary found, then raise error
        if (!Dictionary::singleton_) {
          throw vega::Exception("Dictionary not yet initialized with call to set_dictionary()");
        }
      }

      return *Dictionary::singleton_;
    }

    Dictionary::Dictionary(const std::string& file_name) {
      std::regex public_page_regex("^public \\(([0-9a-fA-FxX]{4}),([0-9a-fA-FxX]{4})\\)\\s+([a-zA-Z]{2}(?:/[a-zA-Z]{2})*)\\s+(\\w+)\\s+([1-9]\\d*(?:-(?:[1-9]\\d*)?n?)?)$");
      std::regex private_page_regex("^private\\s+\"(.+)\"\\s+\\(([0-9a-fA-FxX]{4}),([0-9a-fA-FxX]{4})\\)\\s+([a-zA-Z]{2}(?:/[a-zA-Z]{2})*)\\s+([\\w\\-]+)\\s+\"(.+)\"$");

      std::ifstream file(file_name);
      std::string line;
      std::vector<std::shared_ptr<const Page>> pages;

      // Process each line in the dictionary file
      while (std::getline(file, line)) {
        if (line.length() == 0 || line.at(0) == '#') continue;
        std::smatch line_match;

        if (std::regex_match(line, line_match, public_page_regex)) {
          std::string group_s = line_match[1].str();
          std::string element_s = line_match[2].str();

          const TagMask tag_mask(group_s, element_s);
          const MultiVR multi_vr(line_match[3].str());
          const std::string name = line_match[4].str();
          const VM vm(line_match[5].str());

          auto page = std::make_shared<const Page>(name, tag_mask, multi_vr, vm);
          pages.push_back(page);
        }
        else if (std::regex_match(line, line_match, private_page_regex)) {
          auto it = m_name_to_private_owner.find(line_match[1].str());
          std::shared_ptr<PrivateOwner> private_owner;

          if (it == m_name_to_private_owner.end()) {
            private_owner = std::make_shared<PrivateOwner>(line_match[1].str());
            m_name_to_private_owner.emplace(private_owner->name(), private_owner);
          }
          else {
            private_owner = it->second;
          }

          const TagMask tag_mask(line_match[2].str(), line_match[3].str());
          // FIXME: Allow invalid private tags??
          if (tag_mask.value_tag().group() < 9) continue;
          // FIXME: Allow group masks like 60xx and 70xx
          if (tag_mask.mask_tag() != Tag{0xFFFF,0x00FF}) continue;
          // NOTE:  Why the heck are there "private" elements in gdcm with even groups?
          if ((tag_mask.value_tag().group() & 1) == 0) continue;

          const MultiVR multi_vr(line_match[4].str());
          const std::string name = line_match[6].str();
          const VM vm(line_match[5].str());

          auto private_page = std::make_shared<const Page>(private_owner, name, tag_mask, multi_vr, vm);
          private_owner->add_page(private_page);
        }
        else {
          throw vega::Exception(std::string("Invalid line in dictionary: ") + line);
        }
      }

      // Generic private page
      auto page = std::make_shared<const Page>(std::string("PrivateCreator"), TagMask{0x0001, 0x0001, 0x0000, 0x0000}, MultiVR{"LO"}, VM{1, VM::MAX_LIMIT});
      pages.push_back(page);

      this->add_pages(pages);
    }

    void Dictionary::add_pages(const std::vector<std::shared_ptr<const Page>>& pages) {
      std::set<unsigned> popcounts;

      unsigned popcount;

      for (const auto& page : pages) {
        m_name_to_page.emplace(page->name(), page);

        popcount = page->tag_mask().mask_popcount();
        popcounts.insert(popcount);

        {
          auto it = m_popcount_to_tag_masks.find(popcount);
          if (it == m_popcount_to_tag_masks.end()) {
            m_popcount_to_tag_masks.emplace(popcount, std::set<Tag>());
            it = m_popcount_to_tag_masks.find(popcount);
          }

          it->second.insert(page->tag_mask().mask_tag());
        }

        {
          auto it = m_mask_tag_to_map_from_value_tag_to_pages.find(page->tag_mask().mask_tag());
          if (it == m_mask_tag_to_map_from_value_tag_to_pages.end()) {
            m_mask_tag_to_map_from_value_tag_to_pages.emplace(page->tag_mask().mask_tag(), std::map<Tag,std::shared_ptr<const Page>>());
            it = m_mask_tag_to_map_from_value_tag_to_pages.find(page->tag_mask().mask_tag());
          }

          it->second.emplace(page->tag_mask().value_tag(), page);
        }
      }

      for (const auto& popcount : popcounts) {
        m_popcounts_decreasing.push_back(popcount);
      }

      std::sort(m_popcounts_decreasing.rbegin(), m_popcounts_decreasing.rend());
    }


    // Search by masks,
    // starting with most restrictive (0xFFFFFFFF -- large popcount) and moving
    // down to less restrictive until we have a match
    std::shared_ptr<const Page> Dictionary::page_for(const Tag& tag) const {
      Tag masked_tag;
      for (const auto& popcount : m_popcounts_decreasing) {
        const auto& tag_masks = m_popcount_to_tag_masks.find(popcount);
        for (const Tag& tag_mask : tag_masks->second) {
          masked_tag = Tag(tag_mask.tag() & tag.tag());

          const auto& map_from_tag_value_to_pages = m_mask_tag_to_map_from_value_tag_to_pages.find(tag_mask)->second;
          auto it = map_from_tag_value_to_pages.find(masked_tag);
          if (it != map_from_tag_value_to_pages.end()) {
            return it->second;
          }
        }
      }

      return nullptr;
    }

    std::shared_ptr<const Page> Dictionary::page_for(const std::string& name) const {
      auto it = m_name_to_page.find(name);
      return (it == m_name_to_page.end()) ? nullptr : it->second;
    }

    std::shared_ptr<PrivateOwner> Dictionary::private_owner(const std::string& name) const {
      auto it = m_name_to_private_owner.find(name);
      return (it == m_name_to_private_owner.end()) ? nullptr : it->second;
    }
  }
}
