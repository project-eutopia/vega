#pragma once

#include "vega/tag.h"

#include <string>
#include <map>
#include <memory>

namespace vega {
  namespace dictionary {
    class Page;

    class PrivateOwner {
      private:
        const std::string m_name;

        // Tag of form (gggg, xxee)
        // gggg is the group, and 00ee is the element, where the xx part is a non-unique block identifier.
        // The block identifier is used to distingush between private owners that use the same group.
        // For example,
        // (0009,....) could be SIENET and SPI simultaneously.  In this case, the PrivateOwner
        // tag of (0009,00xx) would carry a string representing the owner name, and the xx would mark
        // the private block that it uses, which would have elements of the form (0009,xxee) where ee
        // uniquely determines the element.  For example:
        //
        // (0009,0010) "SIENET"
        // (0009,0011) "SPI"
        // (0009,1001) "[SIENET] SIENET Command Field"
        // (0009,1110) "[SPI] Comments"
        std::map<Tag::group_type, std::map<Tag::element_type, std::shared_ptr<const Page>>> m_group_to_element_to_page;

      public:
        explicit PrivateOwner(const std::string& name);

        void add_page(std::shared_ptr<const Page> page);
        const std::string& name() const;

        std::shared_ptr<const Page> find_page(const Tag& tag) const;
    };
  }
}
