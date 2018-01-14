#pragma once

#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class OtherDoubleManipulator : public FixedSizeElementManipulator<double> {
      using FixedSizeElementManipulator<double>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::OD;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherDoubleManipulator>(VR::value_type value);
  }
}
