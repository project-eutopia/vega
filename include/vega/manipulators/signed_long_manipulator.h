#pragma once

#include <cstdint>
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class SignedLongManipulator : public FixedSizeElementManipulator<int32_t> {
      using FixedSizeElementManipulator<int32_t>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::SL;
        }
    };
  }
}
