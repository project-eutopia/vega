#include "vega/manipulators/unlimited_characters_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnlimitedCharactersManipulator>(VR::value_type value) {
      return value == vr::UC_VALUE;
    }
  }
}
