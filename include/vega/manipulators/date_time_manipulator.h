#pragma once

#include "vega/date_time.h"
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class DateTimeManipulator : public PaddedStringManipulator<DateTime> {
      using PaddedStringManipulator<DateTime>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::DT;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DateTimeManipulator>(VR::value_type value);
  }
}
