#pragma once

#include "vega/manipulators/single_string_manipulator.h"

namespace vega {
  namespace manipulators {
    class ShortTextManipulator : public SingleStringManipulator {
      using SingleStringManipulator::SingleStringManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::ST;
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::ShortTextManipulator>(VR::value_type value);
  }
}
