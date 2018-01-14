#include "vega/manipulators/other_float_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherFloatManipulator>(VR::value_type value) {
      return value == vr::OF_VALUE;
    }
  }
}
