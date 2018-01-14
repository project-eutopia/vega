#pragma once

#include "vega/types.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    // TODO:  Properly handle different bit encodings
    class PixelDataManipulator : public FixedSizeElementManipulator<Byte> {
      using FixedSizeElementManipulator<Byte>::FixedSizeElementManipulator;

      public:
        static bool allows_vr(const VR& vr) {
          return vr == vr::OB || vr == vr::OW || vr == vr::OX;
        }

        virtual bool is_valid_for(const VR& vr) const override {
          return PixelDataManipulator::allows_vr(vr);
        }
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::PixelDataManipulator>(VR::value_type value);
  }
}
