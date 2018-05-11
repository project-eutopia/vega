#pragma once

#include "vega/range_type.h"

#include <string>
#include <memory>
#include <regex>
#include <iostream>

namespace vega {
  class RegexString;

  class Date {
    private:
      static const std::shared_ptr<const std::regex> SINGLE_REGEX;
      static const std::shared_ptr<const std::regex> RANGE_REGEX;

      // Can either be single time
      std::shared_ptr<const RegexString> m_value;
      // Or a range of Dates
      std::shared_ptr<const Date> m_lower;
      std::shared_ptr<const Date> m_upper;

    public:
      Date();
      explicit Date(const std::string& s);
      Date(const std::shared_ptr<Date>& lower, const std::shared_ptr<Date>& upper);

      bool is_range() const;
      const std::shared_ptr<const Date>& lower() const;
      const std::shared_ptr<const Date>& upper() const;

      bool operator==(const Date& other) const;
      bool operator!=(const Date& other) const;

      std::string str() const;
      friend std::ostream& operator<<(std::ostream& os, const Date& date);
      friend std::istream& operator>>(std::istream& is, Date& date);

      void set_string(const std::string& s);

      template <typename T>
      friend void range_read(std::istream& is, T& t);

      static std::string read_single_string_from(std::istream& is);
  };
}
