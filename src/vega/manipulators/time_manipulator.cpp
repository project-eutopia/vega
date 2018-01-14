#include "vega/manipulators/time_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::TimeManipulator>(VR::value_type value) {
      return value == vr::TM_VALUE;
    }
  }
}
