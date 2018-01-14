#include "vega/manipulators/other_word_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherWordManipulator>(VR::value_type value) {
      return value == vr::OW_VALUE;
    }
  }
}
