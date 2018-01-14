#include "vega/manipulators/long_string_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::LongStringManipulator>(VR::value_type value) {
      return value == vr::LO_VALUE;
    }
  }
}
