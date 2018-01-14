#include "vega/manipulators/short_string_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::ShortStringManipulator>(VR::value_type value) {
      return value == vr::SH_VALUE;
    }
  }
}
