#include "vega/manipulators/unknown_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnknownManipulator>(VR::value_type value) {
      return value == vr::UN_VALUE;
    }
  }
}
