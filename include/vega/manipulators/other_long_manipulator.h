#pragma once

#include "vega/types.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class OtherLongManipulator : public FixedSizeElementManipulator<Long> {
      using FixedSizeElementManipulator<Long>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::OL;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::OtherLongManipulator>(VR::value_type value);
  }
}
