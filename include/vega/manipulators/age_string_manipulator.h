#pragma once

#include "vega/age.h"
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class AgeStringManipulator : public PaddedStringManipulator<Age> {
      using PaddedStringManipulator<Age>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::AS;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::AgeStringManipulator>(VR::value_type value);
  }
}
