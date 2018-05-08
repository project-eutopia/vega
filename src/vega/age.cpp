#include "vega/age.h"
#include "vega/vega.h"
#include <stdexcept>
#include <sstream>

namespace vega {
  Age::Age()
    :
      m_count(0),
      m_unit(Unit::DAY)
  {}

  Age::Age(uint16_t count, Age::Unit unit)
    :
      m_count(count),
      m_unit(unit)
  {
    if (m_count > 999) throw vega::Exception("Age count must be at most 3 digits");
  }

  Age::Age(const std::string& s) {
    if (s.size() > 4) throw vega::Exception("Age string must be at most 4 characters long");
    m_count = std::stoul(s.substr(0, s.size()-1));
    char unit = s[s.size()-1];
    m_unit = Age::unit_from_char(unit);
  }

  Age::Unit Age::unit_from_char(char c) {
    switch (c) {
      case 'D':
        return Unit::DAY;
      case 'W':
        return Unit::WEEK;
      case 'M':
        return Unit::MONTH;
      case 'Y':
        return Unit::YEAR;
      default:
        throw vega::Exception(std::string("Unknown age unit: ") + c);
    }
  }

  uint16_t Age::count() const { return m_count; }
  Age::Unit Age::unit() const { return m_unit; }

  std::string Age::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  std::ostream& operator<<(std::ostream& os, const Age& age) {
    if (age.count() < 100) os << '0';
    if (age.count() < 10)  os << '0';
    os << age.count() << static_cast<char>(age.unit());
    return os;
  }

  std::istream& operator>>(std::istream& is, Age& age) {
    is >> age.m_count;
    char c;
    is >> c;
    age.m_unit = Age::unit_from_char(c);
    return is;
  }

  bool Age::operator==(const Age& other) const {
    return this->count() == other.count() && this->unit() == other.unit();
  }

  bool Age::operator!=(const Age& other) const {
    return !(*this == other);
  }
}
