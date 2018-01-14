#pragma once

#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class OtherFloatManipulator : public FixedSizeElementManipulator<float> {
      using FixedSizeElementManipulator<float>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::OF;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherFloatManipulator>(VR::value_type value);
  }
}
