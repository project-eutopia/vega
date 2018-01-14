#include "vega/manipulators/age_string_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::AgeStringManipulator>(VR::value_type value) {
      return value == vr::AS_VALUE;
    }
  }
}
