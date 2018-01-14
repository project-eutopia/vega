#pragma once

#include "vega/tag.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    class AttributeTagManipulator : public FixedSizeElementManipulator<Tag> {
      using FixedSizeElementManipulator<Tag>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::AT;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::AttributeTagManipulator>(VR::value_type value);
  }
}
