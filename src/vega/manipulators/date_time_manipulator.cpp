#include "vega/manipulators/date_time_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DateTimeManipulator>(VR::value_type value) {
      return value == vr::DT_VALUE;
    }
  }
}
