#pragma once

#include <cstdint>
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class UnsignedLongManipulator : public FixedSizeElementManipulator<uint32_t> {
      using FixedSizeElementManipulator<uint32_t>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::UL;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnsignedLongManipulator>(VR::value_type value);
  }
}
