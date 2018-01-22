#pragma once

#include <string>
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class PersonNameManipulator : public PaddedStringManipulator<std::string> {
      using PaddedStringManipulator<std::string>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::PN;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::PersonNameManipulator>(VR::value_type value);
  }
}
