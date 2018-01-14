#include "vega/manipulators/date_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DateManipulator>(VR::value_type value) {
      return value == vr::DA_VALUE;
    }
  }
}
