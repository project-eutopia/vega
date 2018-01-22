#pragma once

#include <cstdint>
#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  class IntegerString {
    private:
      int m_number;

    public:
      IntegerString();
      IntegerString(int i);
      IntegerString(const std::string& s);

      operator int() const;

      bool operator ==(const IntegerString& is) const;
      bool operator !=(const IntegerString& is) const;

      std::string str() const;
  };

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
