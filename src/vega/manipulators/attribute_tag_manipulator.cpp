#include "vega/manipulators/attribute_tag_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::AttributeTagManipulator>(VR::value_type value) {
      return value == vr::AT_VALUE;
    }
  }
}
