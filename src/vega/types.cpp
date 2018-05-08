#include "vega/types.h"
#include "vega/math.h"
#include "vega/string.h"

#include <sstream>
#include <iomanip>

namespace vega {
  bool Byte::operator==(const Byte& other) const {
    return this->u == other.u;
  }

  bool Byte::operator!=(const Byte& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Byte& byte) {
    os << vega::to_string(byte);
    return os;
  }

  std::istream& operator>>(std::istream& is, Byte& byte) {
    if (is.peek() == '-')
      is >> byte.i;
    else {
      unsigned u;
      is >> u;
      byte.u = u;
    }
    return is;
  }

  bool Word::operator==(const Word& other) const {
    return this->u == other.u;
  }

  bool Word::operator!=(const Word& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Word& word) {
    os << vega::to_string(word);
    return os;
  }

  std::istream& operator>>(std::istream& is, Word& word) {
    if (is.peek() == '-')
      is >> word.i;
    else {
      unsigned u;
      is >> u;
      word.u = u;
    }
    return is;
  }

  bool Long::operator==(const Long& other) const {
    return this->u == other.u;
  }

  bool Long::operator!=(const Long& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Long& l) {
    os << vega::to_string(l);
    return os;
  }

  std::istream& operator>>(std::istream& is, Long& l) {
    if (is.peek() == '-')
      is >> l.i;
    else {
      unsigned u;
      is >> u;
      l.u = u;
    }
    return is;
  }

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

  std::ostream& operator<<(std::ostream& os, const DecimalString& ds) {
    std::ios::fmtflags f( os.flags() );

    int exponent;
    double mantissa = math::frexp10(ds.m_double, &exponent);

    if (exponent >= 100) {
      // Exponent size e100 = 4
      if (mantissa > 0) {
        os << std::setprecision(11) << mantissa << 'e' << exponent;
      }
      else {
        os << std::setprecision(10) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= 13) {
      // Exponent size e10 = 3
      if (mantissa > 0) {
        os << std::setprecision(12) << mantissa << 'e' << exponent;
      }
      else {
        os << std::setprecision(11) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= 0) {
      os << std::setprecision(14) << ds.m_double;
    }
    else if (exponent >= -9) {
      if (mantissa > 0) {
        os << std::setprecision(12) << mantissa << 'e' << exponent;
      }
      else {
        os << std::setprecision(11) << mantissa << 'e' << exponent;
      }
    }
    else if (exponent >= -99) {
      if (mantissa > 0) {
        os << std::setprecision(11) << mantissa << 'e' << exponent;
      }
      else {
        os << std::setprecision(10) << mantissa << 'e' << exponent;
      }
    }
    else {
      if (mantissa > 0) {
        os << std::setprecision(10) << mantissa << 'e' << exponent;
      }
      else {
        os << std::setprecision(9) << mantissa << 'e' << exponent;
      }
    }

    os.flags(f);
    return os;
  }

  std::istream& operator>>(std::istream& is, DecimalString& ds) {
    is >> ds.m_double;
    return is;
  }

  // Maximum length 16
  std::string DecimalString::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }
}
