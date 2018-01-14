#pragma once

#include "vega/dictionary/private_owner_block.h"

namespace vega {
  namespace dictionary {
    class PrivateOwnerBlocks {
      private:
        std::map<Tag::group_type, std::map<Tag::element_type, std::shared_ptr<PrivateOwnerBlock>>> m_group_to_element_to_block;

      public:
        explicit PrivateOwnerBlocks();

        void add_private_owner_block(std::shared_ptr<PrivateOwnerBlock> block);
        std::shared_ptr<PrivateOwnerBlock> find_block(const Tag& tag) const;
    };
  }
}
