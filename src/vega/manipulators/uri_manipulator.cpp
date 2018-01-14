#include "vega/manipulators/uri_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::URIManipulator>(VR::value_type value) {
      return value == vr::UR_VALUE;
    }
  }
}
