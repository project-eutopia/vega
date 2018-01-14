#include "vega/age.h"
#include "vega/vega.h"
#include <stdexcept>

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
    if (this->count() < 10) {
      return std::string("00") + std::to_string(this->count()) + static_cast<char>(this->unit());
    }
    else if (this->count() < 100) {
      return std::string("0") + std::to_string(this->count()) + static_cast<char>(this->unit());
    }
    else {
      return std::to_string(this->count()) + static_cast<char>(this->unit());
    }
  }

  bool Age::operator==(const Age& other) const {
    return this->count() == other.count() && this->unit() == other.unit();
  }

  bool Age::operator!=(const Age& other) const {
    return !(*this == other);
  }
}
