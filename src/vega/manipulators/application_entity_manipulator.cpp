#include "vega/manipulators/application_entity_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::ApplicationEntityManipulator>(VR::value_type value) {
      return value == vr::AE_VALUE;
    }
  }
}
