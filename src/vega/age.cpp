#include "vega/age.h"
#include "vega/vega.h"
#include <stdexcept>
#include <sstream>

namespace vega {
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

    switch (unit) {
      case 'D':
        m_unit = Unit::DAY;
        break;
      case 'W':
        m_unit = Unit::WEEK;
        break;
      case 'M':
        m_unit = Unit::MONTH;
        break;
      case 'Y':
        m_unit = Unit::YEAR;
        break;
      default:
        throw vega::Exception(std::string("Unknown age unit: ") + unit);
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

  bool Age::operator==(const Age& other) const {
    return this->count() == other.count() && this->unit() == other.unit();
  }

  bool Age::operator!=(const Age& other) const {
    return !(*this == other);
  }
}
