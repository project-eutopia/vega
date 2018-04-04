#pragma once

#include <stdint.h>
#include <string>
#include <ostream>

namespace vega {
  class Age {
    public:
      enum class Unit : char {
        DAY   = 'D',
        WEEK  = 'W',
        MONTH = 'M',
        YEAR  = 'Y'
      };

    private:
      uint16_t m_count;
      Unit m_unit;

    public:
      Age(uint16_t count, Unit unit);
      explicit Age(const std::string& s);

      uint16_t count() const;
      Unit unit() const;
      std::string str() const;
      friend std::ostream& operator<<(std::ostream& os, const Age& age);

      bool operator==(const Age& other) const;
      bool operator!=(const Age& other) const;
  };
}
