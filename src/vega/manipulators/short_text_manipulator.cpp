#include "vega/manipulators/short_text_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::ShortTextManipulator>(VR::value_type value) {
      return value == vr::ST_VALUE;
    }
  }
}
