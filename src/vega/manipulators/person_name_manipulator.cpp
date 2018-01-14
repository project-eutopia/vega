#include "vega/manipulators/person_name_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::PersonNameManipulator>(VR::value_type value) {
      return value == vr::PN_VALUE;
    }
  }
}
