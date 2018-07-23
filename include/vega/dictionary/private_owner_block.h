#pragma once

#include "vega/dictionary/private_owner.h"
#include <memory>

namespace vega {
  namespace dictionary {
    class PrivateOwnerBlock {
      private:
        const std::shared_ptr<const PrivateOwner> m_private_owner;
        const Tag::group_type m_group;
        const Tag::element_type m_block_number;

      public:
        explicit PrivateOwnerBlock(const std::shared_ptr<PrivateOwner>& private_owner, Tag::group_type group, Tag::element_type block_number);

        const PrivateOwner& private_owner() const;
        const Tag::group_type& group() const;
        // Of form (xx00)
        const Tag::element_type& block_number() const;
    };
  }
}
