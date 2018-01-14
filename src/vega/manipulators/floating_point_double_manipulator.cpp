#include "vega/manipulators/floating_point_double_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::FloatingPointDoubleManipulator>(VR::value_type value) {
      return value == vr::FD_VALUE;
    }
  }
}
