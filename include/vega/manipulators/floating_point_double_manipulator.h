#pragma once

#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class FloatingPointDoubleManipulator : public FixedSizeElementManipulator<double> {
      using FixedSizeElementManipulator<double>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::FD;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::FloatingPointDoubleManipulator>(VR::value_type value);
  }
}
