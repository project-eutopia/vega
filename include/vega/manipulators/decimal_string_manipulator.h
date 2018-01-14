#pragma once

#include "vega/manipulators/padded_string_manipulator.h"

namespace vega {
  class DecimalString {
    private:
      double m_double;

    public:
      explicit DecimalString();

      // Allow conversion from arbitrary type if allowed
      template <typename T>
      DecimalString(const T& t) : m_double(t) {}
      DecimalString(const std::string& s);

      operator double() const;
      operator float() const;

      bool operator ==(const DecimalString& ds) const;
      bool operator !=(const DecimalString& ds) const;

      std::string str() const;
  };

  namespace manipulators {
    class DecimalStringManipulator : public PaddedStringManipulator<DecimalString, ' ', '\\'> {
      using PaddedStringManipulator<DecimalString, ' ', '\\'>::PaddedStringManipulator;

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
