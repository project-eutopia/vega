#include "vega/dictionary/private_owner.h"
#include "vega/dictionary/page.h"

namespace vega {
  namespace dictionary {
    PrivateOwner::PrivateOwner(const std::string& name)
      : m_name(name)
    {}

    void PrivateOwner::add_page(std::shared_ptr<const Page> page) {
      Tag::group_type   group   = page->tag_mask().value_tag().group();
      Tag::element_type element = page->tag_mask().value_tag().element();

      auto it = m_group_to_element_to_page.find(group);
      if (it == m_group_to_element_to_page.end()) {
        m_group_to_element_to_page.emplace(group, std::map<Tag::element_type, std::shared_ptr<const Page>>());
        it = m_group_to_element_to_page.find(group);
      }

      it->second.emplace(element, page);
    }

    const std::string& PrivateOwner::name() const { return m_name; }

    std::shared_ptr<const Page> PrivateOwner::find_page(const Tag& tag) const {
      auto it = m_group_to_element_to_page.find(tag.group());
      if (it == m_group_to_element_to_page.end()) return nullptr;

      // Private tags of form (xxxx,12xx) where 12 is the owner element
      auto it2 = it->second.find(tag.element() & 0xFF);

      return (it2 == it->second.end()) ? nullptr : it2->second;
    }
  }
}
