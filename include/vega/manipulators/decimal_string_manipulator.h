#pragma once

#include "vega/types.h"
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class DecimalStringManipulator : public PaddedStringManipulator<DecimalString> {
      using PaddedStringManipulator<DecimalString>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::DS;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DecimalStringManipulator>(VR::value_type value);
  }
}
