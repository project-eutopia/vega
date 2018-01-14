#include "vega/manipulators/unsigned_short_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnsignedShortManipulator>(VR::value_type value) {
      return value == vr::US_VALUE;
    }
  }
}
