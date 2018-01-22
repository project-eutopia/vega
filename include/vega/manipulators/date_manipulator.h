#pragma once

#include "vega/date.h"
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class DateManipulator : public PaddedStringManipulator<Date> {
      using PaddedStringManipulator<Date>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::DA;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DateManipulator>(VR::value_type value);
  }
}
