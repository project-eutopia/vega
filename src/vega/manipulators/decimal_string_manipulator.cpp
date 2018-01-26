#include "vega/manipulators/decimal_string_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DecimalStringManipulator>(VR::value_type value) {
      return value == vr::DS_VALUE;
    }
  }
}
