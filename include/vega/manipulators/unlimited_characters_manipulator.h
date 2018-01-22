#pragma once

#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class UnlimitedCharactersManipulator : public PaddedStringManipulator<std::string> {
      using PaddedStringManipulator<std::string>::PaddedStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::UC;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UnlimitedCharactersManipulator>(VR::value_type value);
  }
}
