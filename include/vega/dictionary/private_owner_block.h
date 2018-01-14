#pragma once

#include "vega/dictionary/private_owner.h"

namespace vega {
  namespace dictionary {
    class PrivateOwnerBlock {
      private:
        std::shared_ptr<PrivateOwner> m_private_owner;
        Tag::group_type m_group;
        Tag::element_type m_block_number;

      public:
        explicit PrivateOwnerBlock(std::shared_ptr<PrivateOwner> private_owner, Tag::group_type group, Tag::element_type block_number);

        const std::shared_ptr<PrivateOwner>& private_owner() const;
        const Tag::group_type& group() const;
        // Of form (xx00)
        const Tag::element_type& block_number() const;
    };
  }
}
