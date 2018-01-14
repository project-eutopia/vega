#include "vega/manipulators/unlimited_text_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnlimitedTextManipulator>(VR::value_type value) {
      return value == vr::UT_VALUE;
    }
  }
}
