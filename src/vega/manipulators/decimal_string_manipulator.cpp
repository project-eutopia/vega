#include "vega/manipulators/decimal_string_manipulator.h"
#include "vega/math.h"

#include <sstream>
#include <iomanip>

namespace vega {
  DecimalString::DecimalString() : m_double(0)
  {}

  DecimalString::DecimalString(const std::string& s) : m_double(std::stod(s))
  {}

  DecimalString::operator double() const {
    return m_double;
  }

  DecimalString::operator float() const {
    return m_double;
  }

  bool DecimalString::operator ==(const DecimalString& ds) const {
    return m_double == double(ds);
  }

  bool DecimalString::operator !=(const DecimalString& ds) const {
    return m_double != double(ds);
  }

  // Maximum length 16
  std::string DecimalString::str() const {
    if (m_double == 0) return std::string("0");

    int exponent;
    double mantissa = math::frexp10(m_double, &exponent);

    std::stringstream ss;

    if (exponent >= 100) {
      // Exponent size e100 = 4
      if (mantissa > 0) {
        ss << std::setprecision(11) << mantissa << 'e' << exponent;
      }
      else {
        ss << std::setprecision(10) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= 13) {
      // Exponent size e10 = 3
      if (mantissa > 0) {
        ss << std::setprecision(12) << mantissa << 'e' << exponent;
      }
      else {
        ss << std::setprecision(11) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= 0) {
      ss << std::setprecision(14) << m_double;
    }
    else if (exponent >= -9) {
      if (mantissa > 0) {
        ss << std::setprecision(12) << mantissa << 'e' << exponent;
      }
      else {
        ss << std::setprecision(11) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= -99) {
      if (mantissa > 0) {
        ss << std::setprecision(11) << mantissa << 'e' << exponent;
      }
      else {
        ss << std::setprecision(10) << mantissa << 'e' << exponent;
      }
    }
    else {
      if (mantissa > 0) {
        ss << std::setprecision(10) << mantissa << 'e' << exponent;
      }
      else {
        ss << std::setprecision(9) << mantissa << 'e' << exponent;
      }
    }

    return ss.str();
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::DecimalStringManipulator>(VR::value_type value) {
      return value == vr::DS_VALUE;
    }
  }
}
