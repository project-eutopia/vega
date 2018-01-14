#include "vega/manipulators/pixel_data_manipulator.h"

namespace vega {
  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::PixelDataManipulator>(VR::value_type value) {
      return manipulators::PixelDataManipulator::allows_vr(VR{VR::Data{.value = value}});
    }
  }
}
