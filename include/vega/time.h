#pragma once

#include "vega/range_type.h"

#include <string>
#include <memory>
#include <regex>
#include <ostream>

namespace vega {
  class RegexString;

  /**
   * \brief A class which wraps a Time type (VR of vr::TM).
   *
   * A string of characters of the format hhmmss.frac; where hh contains hours (range
   * 00 to 23), mm contains minutes (range 00 to 59), ss contains seconds (range 00 to 59),
   * and frac contains a fractional part of a second as small as 1 millionth of a second
   * (range 000000 to 999999).  A 24 hour clock is assumed.  Midnight can be represented
   * by only 0000 since 2400 would violate the hour range.  The string may be padded with
   * trailing spaces.  Leading and embedded spaces are not allowed.  One or more of the
   * components mm, ss, or frac may be unspecified as long as every component to the right
   * of an unspecified component is also unspecified.  If frac is unspecified the preceding
   * '.' may not be included.
   */
  class Time {
    public:
      static const std::shared_ptr<const std::regex> SINGLE_REGEX;
      static const std::shared_ptr<const std::regex> RANGE_REGEX;

      // Can either be single Time
      std::shared_ptr<const RegexString> m_value;
      // Or a range of Times
      std::shared_ptr<const Time> m_lower;
      std::shared_ptr<const Time> m_upper;

    public:
      Time();
      explicit Time(const std::string& s);
      Time(const std::shared_ptr<Time>& lower, const std::shared_ptr<Time>& upper);

      bool is_range() const;
      const std::shared_ptr<const Time>& lower() const;
      const std::shared_ptr<const Time>& upper() const;

      bool operator==(const Time& other) const;
      bool operator!=(const Time& other) const;

      std::string str() const;
      friend std::ostream& operator<<(std::ostream& os, const Time& time);
      friend std::istream& operator>>(std::istream& is, Time& time);

      void set_string(const std::string& s);

      template <typename T>
      friend void range_read(std::istream& is, T& t);

      static std::string read_single_string_from(std::istream& is);
  };
}
