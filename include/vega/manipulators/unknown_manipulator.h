#pragma once

#include "vega/types.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class UnknownManipulator : public FixedSizeElementManipulator<Byte> {
      using FixedSizeElementManipulator<Byte>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::UN;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnknownManipulator>(VR::value_type value);
  }
}
