#include "vega/manipulators/signed_short_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::SignedShortManipulator>(VR::value_type value) {
      return value == vr::SS_VALUE;
    }
  }
}
