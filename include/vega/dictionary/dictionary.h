#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "vega/tag_mask.h"
#include "vega/dictionary/page.h"
#include "vega/dictionary/private_owner.h"

namespace vega {
  namespace dictionary {
    class Dictionary {
      private:
        static std::unique_ptr<Dictionary> singleton_;

        std::map<std::string, std::shared_ptr<const Page>> m_name_to_page;
        std::vector<unsigned> m_popcounts_decreasing;
        std::map<unsigned, std::set<Tag>> m_popcount_to_tag_masks;
        std::map<Tag, std::map<Tag,std::shared_ptr<const Page>>> m_mask_tag_to_map_from_value_tag_to_pages;

        std::map<std::string, std::shared_ptr<PrivateOwner>> m_name_to_private_owner;

        explicit Dictionary(const std::string& file_name);
        void add_pages(const std::vector<std::shared_ptr<const Page>>& pages);

      public:
        // Uncopyable
        Dictionary(const Dictionary&) = delete;
        Dictionary& operator=(const Dictionary&) = delete;

        // Unmovable
        Dictionary(Dictionary&&) = delete;
        Dictionary& operator=(Dictionary&&) = delete;

        static void set_dictionary(const std::string& file_name);
        // Using a function instead of a constant will allow updating to
        // handle default locations that are, for instance, platform dependent
        static const std::string& default_dictionary_file_name();
        /**
         * Returns a reference to the existing Dictionary object instance if
         * it has been initialized by set_dictionary(), or else it initializes
         * it if \p allow_default is true to the file given by default_dictionary_file_name().
         */
        static const Dictionary& instance(bool allow_default = true);

        std::shared_ptr<const Page> page_for(const Tag& tag) const;
        std::shared_ptr<const Page> page_for(const std::string& name) const;
        std::shared_ptr<PrivateOwner> private_owner(const std::string& name) const;
    };

    void set_dictionary(const std::string& file_name);
    const Dictionary& instance(bool allow_default = true);
  }
}
