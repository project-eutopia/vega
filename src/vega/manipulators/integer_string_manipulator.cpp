#include "vega/manipulators/integer_string_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::IntegerStringManipulator>(VR::value_type value) {
      return value == vr::IS_VALUE;
    }
  }
}
