#pragma once

#include <cstdint>
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class UnsignedShortManipulator : public FixedSizeElementManipulator<uint16_t> {
      using FixedSizeElementManipulator<uint16_t>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::US;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnsignedShortManipulator>(VR::value_type value);
  }
}
