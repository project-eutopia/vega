#include "vega/dictionary/page.h"

#include <string>

#include "vega/dictionary/private_owner.h"
#include "vega/tag_mask.h"
#include "vega/multi_vr.h"
#include "vega/vm.h"

namespace vega {
  namespace dictionary {
    Page::Page(const std::string& name, const TagMask& tag_mask, const MultiVR& multi_vr, const VM& vm)
      :
        m_owner(),
        m_name(name),
        m_tag_mask(tag_mask),
        m_multi_vr(multi_vr),
        m_vm(vm)
    {}

    Page::Page(std::shared_ptr<const PrivateOwner> owner, const std::string& name, const TagMask& tag_mask, const MultiVR& multi_vr, const VM& vm)
      :
        m_owner(owner),
        m_name(name),
        m_tag_mask(tag_mask),
        m_multi_vr(multi_vr),
        m_vm(vm)
    {
      // Must have form of (1359,xx24), and odd group above 9
      if (m_tag_mask.mask_tag() != Tag{0xFFFF, 0x00FF} || (m_tag_mask.value_tag().group() & 1) == 0 || m_tag_mask.value_tag().group() < 9) {
        throw vega::Exception("Private Page encountered an invalid tag mask: " + m_tag_mask.value_tag().str() + ", " + m_tag_mask.mask_tag().str());
      }
    }

    const std::string& Page::name() const { return m_name; }
    const TagMask& Page::tag_mask() const { return m_tag_mask; }
    const MultiVR& Page::multi_vr() const { return m_multi_vr; }
    const VM& Page::vm() const { return m_vm; }

    bool Page::allows_vr(const VR& vr) const {
      // Always allow anything for private tags
      if (this->tag_mask().is_private()) return true;
      return this->multi_vr().contains(vr);
    }

    bool Page::contains(const Tag& tag) const { return this->tag_mask().contains(tag); }

    VR Page::determine_implicit_vr() const {
      // If unambiguous, return VR
      if (this->multi_vr().single()) return this->multi_vr().vrs()[0];

      // Overlay data is OW in implicit VR mode
      if (this->name() == std::string("OverlayData")) return vr::OW;
      // Pixel data and waveform data is OW in implicit VR mode
      if (this->tag_mask().contains(Tag::PIXEL_DATA) || this->name() == std::string("WaveformData")) {
        return vr::OW;
      }

      // Private
      if (this->tag_mask().value_tag().tag() & 1) {
        return vr::LO;
      }

      return this->multi_vr().to_single_vr();
    }

    std::weak_ptr<const PrivateOwner> Page::private_owner() const { return m_owner; }
  }
}
