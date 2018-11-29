#pragma once

#include <regex>
#include <string>

namespace vega {
  // min, max used to denote VM of "min-max"
  class VM {
    public:
      static const unsigned MAX_LIMIT;

      typedef uint32_t limit_type;

    private:
      // 1 => limits = (1,1), multiple = 0
      // 2-4 => limits = (2,4), multiple = 0
      // 3-n => limits = (3,1), multiple = 1
      // 4-4n => limits = (4,4), multiple = 1
      const std::pair<limit_type, limit_type> m_limits;
      const bool m_multiple;

    public:
      constexpr VM() : m_limits(1, 1), m_multiple(false) {}
      explicit constexpr VM(limit_type n) : m_limits(n, n), m_multiple(false) {}
      explicit VM(const std::string& vm);
      constexpr VM(limit_type min, limit_type max, bool multiple = false)
        : m_limits(min, max),
          m_multiple(multiple)
      {}

      const std::pair<limit_type, limit_type>& limits() const;
      bool multiple() const;

      // True if there is only one possibility
      bool is_single() const;
      bool allows_multiplicity(limit_type multiplicity) const;

      friend std::ostream& operator<<(std::ostream& os, const VM& vm);

    private:
      static const std::regex REGEX;
      static std::pair<limit_type,limit_type> get_pair(const std::string& vm);
  };
}
