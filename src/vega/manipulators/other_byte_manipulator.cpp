#include "vega/manipulators/other_byte_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherByteManipulator>(VR::value_type value) {
      return value == vr::OB_VALUE;
    }
  }
}
