#pragma once

#include <string>

#include "vega/tag_mask.h"
#include "vega/multi_vr.h"
#include "vega/vm.h"

namespace vega {
  namespace dictionary {
    class PrivateOwner;

    class Page {
      private:
        std::weak_ptr<const PrivateOwner> m_owner;
        std::string m_name;
        TagMask m_tag_mask;
        MultiVR m_multi_vr;
        VM m_vm;

      public:
        Page(const std::string& name, const TagMask& tag_mask, const MultiVR& multi_vr, const VM& vm);
        Page(std::shared_ptr<const PrivateOwner> owner, const std::string& name, const TagMask& tag_mask, const MultiVR& multi_vr, const VM& vm);

        const std::string& name() const;
        const TagMask& tag_mask() const;
        const MultiVR& multi_vr() const;
        const VM& vm() const;

        bool allows_vr(const VR& vr) const;
        bool contains(const Tag& tag) const;
        VR determine_implicit_vr() const;

        std::weak_ptr<const PrivateOwner> private_owner() const;
    };
  }
}
