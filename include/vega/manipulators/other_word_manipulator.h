#pragma once

#include <cstdint>
#include "vega/types.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class OtherWordManipulator : public FixedSizeElementManipulator<Word> {
      using FixedSizeElementManipulator<Word>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::OW;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherWordManipulator>(VR::value_type value);
  }
}
