#include "vega/manipulators/unsigned_long_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnsignedLongManipulator>(VR::value_type value) {
      return value == vr::UL_VALUE;
    }
  }
}
