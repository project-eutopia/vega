#include "vega/manipulators/floating_point_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::FloatingPointManipulator>(VR::value_type value) {
      return value == vr::FL_VALUE;
    }
  }
}
