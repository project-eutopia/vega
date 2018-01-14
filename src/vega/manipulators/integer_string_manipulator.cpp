#include "vega/manipulators/integer_string_manipulator.h"
#include "vega/math.h"

#include <sstream>
#include <iomanip>

namespace vega {
  IntegerString::IntegerString() : m_number(0)
  {}

  IntegerString::IntegerString(int i) : m_number(i)
  {}

  IntegerString::IntegerString(const std::string& s) : m_number(std::stoi(s))
  {}

  IntegerString::operator int() const {
    return m_number;
  }

  bool IntegerString::operator ==(const IntegerString& ds) const {
    return m_number == int(ds);
  }

  bool IntegerString::operator !=(const IntegerString& ds) const {
    return m_number != int(ds);
  }

  // Maximum length 12
  std::string IntegerString::str() const {
    return std::to_string(m_number);
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::IntegerStringManipulator>(VR::value_type value) {
      return value == vr::IS_VALUE;
    }
  }
}
