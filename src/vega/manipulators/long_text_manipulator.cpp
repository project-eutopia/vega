#include "vega/manipulators/long_text_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::LongTextManipulator>(VR::value_type value) {
      return value == vr::LT_VALUE;
    }
  }
}
