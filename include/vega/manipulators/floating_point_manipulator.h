#pragma once

#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class FloatingPointManipulator : public FixedSizeElementManipulator<float> {
      using FixedSizeElementManipulator<float>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::FL;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::FloatingPointManipulator>(VR::value_type value);
  }
}
