#include "vega/manipulators/other_double_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherDoubleManipulator>(VR::value_type value) {
      return value == vr::OD_VALUE;
    }
  }
}
