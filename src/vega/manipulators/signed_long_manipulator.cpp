#include "vega/manipulators/signed_long_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::SignedLongManipulator>(VR::value_type value) {
      return value == vr::SL_VALUE;
    }
  }
}
