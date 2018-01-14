#pragma once

#include <set>
#include <map>
#include <vector>

#include "vega/tag_mask.h"
#include "vega/dictionary/page.h"
#include "vega/dictionary/private_owner.h"

namespace vega {
  namespace dictionary {
    class Dictionary {
      private:
        static std::shared_ptr<Dictionary> singleton;

        std::map<std::string, std::shared_ptr<const Page>> m_name_to_page;
        std::vector<unsigned> m_popcounts_decreasing;
        std::map<unsigned, std::set<Tag>> m_popcount_to_tag_masks;
        std::map<Tag, std::map<Tag,std::shared_ptr<const Page>>> m_mask_tag_to_map_from_value_tag_to_pages;

        std::map<std::string, std::shared_ptr<PrivateOwner>> m_name_to_private_owner;

        explicit Dictionary(const std::string& file_name);
        void add_pages(const std::vector<std::shared_ptr<const Page>>& pages);

      public:
        static void set_dictionary(const std::string& file_name);
        static const Dictionary& instance();

        std::shared_ptr<const Page> page_for(const Tag& tag) const;
        std::shared_ptr<const Page> page_for(const std::string& name) const;
        std::shared_ptr<PrivateOwner> private_owner(const std::string& name) const;
    };

    void set_dictionary(const std::string& file_name);
    const Dictionary& instance();
  }
}
