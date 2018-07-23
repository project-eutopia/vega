#include "vega/dictionary/private_owner_block.h"

namespace vega {
  namespace dictionary {
    PrivateOwnerBlock::PrivateOwnerBlock(const std::shared_ptr<PrivateOwner>& private_owner, Tag::group_type group, Tag::element_type block_number)
      :
        m_private_owner(std::const_pointer_cast<const PrivateOwner>(private_owner)),
        m_group(group),
        m_block_number(block_number << 8)
    {}

    const PrivateOwner& PrivateOwnerBlock::private_owner() const { return *m_private_owner; }
    const Tag::group_type& PrivateOwnerBlock::group() const { return m_group; }
    const Tag::element_type& PrivateOwnerBlock::block_number() const { return m_block_number; }
  }
}
