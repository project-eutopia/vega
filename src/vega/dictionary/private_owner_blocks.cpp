#include "vega/dictionary/private_owner_blocks.h"

namespace vega {
  namespace dictionary {
    PrivateOwnerBlocks::PrivateOwnerBlocks() {}

    void PrivateOwnerBlocks::add_private_owner_block(std::shared_ptr<PrivateOwnerBlock> block) {
      Tag::group_type   group        = block->group();
      Tag::element_type block_number = block->block_number();

      auto it = m_group_to_element_to_block.find(group);
      if (it == m_group_to_element_to_block.end()) {
        m_group_to_element_to_block.emplace(group, std::map<Tag::element_type, std::shared_ptr<PrivateOwnerBlock>>());
        it = m_group_to_element_to_block.find(group);
      }

      it->second.emplace(block_number, block);
    }

    std::shared_ptr<PrivateOwnerBlock> PrivateOwnerBlocks::find_block(const Tag& tag) const {
      auto it = m_group_to_element_to_block.find(tag.group());
      if (it == m_group_to_element_to_block.end()) return nullptr;

      // Block number is (xx00) part of element
      auto it2 = it->second.find(tag.element() & 0xFF00);
      return (it2 == it->second.end()) ? nullptr : it2->second;
    }
  }
}
