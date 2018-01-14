#include "vega/manipulators/other_long_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherLongManipulator>(VR::value_type value) {
      return value == vr::OL_VALUE;
    }
  }
}
