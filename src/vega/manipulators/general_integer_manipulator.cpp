#include "vega/manipulators/general_integer_manipulator.h"

#include <algorithm>

#include "vega/manipulators/other_byte_manipulator.h"
#include "vega/manipulators/other_word_manipulator.h"
#include "vega/manipulators/signed_short_manipulator.h"
#include "vega/manipulators/unsigned_short_manipulator.h"

namespace vega {
  namespace manipulators {
    bool GeneralIntegerManipulator::is_valid_for(const VR& vr) const  {
      return vr::manipulator_is_valid_for<GeneralIntegerManipulator>(vr.data().value);
    }

    // Updates VR value
    std::shared_ptr<ValueManipulator> GeneralIntegerManipulator::to_specific_manipulator(VR& vr) const {
      auto minmax = std::minmax_element(this->begin(), this->end());
      auto min = *minmax.first;
      auto max = *minmax.second;

      bool has_negatives = min < 0;

      // OB or OW type
      if (vr == vr::OX) {
        if (has_negatives) return nullptr;

        if (max < 256) {
          vr = vr::OB;
          return this->coerce_to_manipulator<OtherByteManipulator>();
        }
        else if (max < 65536) {
          vr = vr::OW;
          return this->coerce_to_manipulator<OtherWordManipulator>();
        }
      }
      // SS or US type
      // SS or US or OW type (favor US over OW, because why not)
      else if (vr == vr::XS || vr == vr::XW) {
        if (has_negatives) {
          if (min >= -32768 && max < 32768) {
            vr = vr::SS;
            return this->coerce_to_manipulator<SignedShortManipulator>();
          }
        }
        else {
          if (max < 65536) {
            vr = vr::US;
            return this->coerce_to_manipulator<UnsignedShortManipulator>();
          }
        }
      }
      // US or OW type
      else if (vr == vr::UW) {
        if (has_negatives) return nullptr;
        if (max < 65536) {
          vr = vr::US;
          return this->coerce_to_manipulator<UnsignedShortManipulator>();
        }
      }

      return nullptr;
    }

    template <>
    Byte GeneralIntegerManipulator::coerce_at<Byte>(size_t i) const {
      return Byte{.u = static_cast<uint8_t>(this->at(i))};
    }

    template <>
    Word GeneralIntegerManipulator::coerce_at<Word>(size_t i) const {
      return Word{.u = static_cast<uint16_t>(this->at(i))};
    }
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::GeneralIntegerManipulator>(VR::value_type value) {
      if (value == vr::OX_VALUE || value == vr::OX_VALUE || value == vr::XW_VALUE || value == vr::UW_VALUE) {
        return true;
      }
      else {
        return false;
      }
    }
  }
}
