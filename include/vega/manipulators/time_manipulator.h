#pragma once

#include "vega/time.h"
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class TimeManipulator : public PaddedStringManipulator<Time> {
      using PaddedStringManipulator<Time>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::TM;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::TimeManipulator>(VR::value_type value);
  }
}
