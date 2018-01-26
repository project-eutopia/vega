#pragma once

#include <cstdint>
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class IntegerStringManipulator : public PaddedStringManipulator<int32_t> {
      using PaddedStringManipulator<int32_t>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::IS;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::IntegerStringManipulator>(VR::value_type value);
  }
}
