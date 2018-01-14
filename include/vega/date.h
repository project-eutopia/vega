#pragma once

#include <string>
#include <memory>
#include <regex>

namespace vega {
  class RegexString;

  class Date {
    private:
      static const std::shared_ptr<const std::regex> SINGLE_DATE_REGEX;
      static const std::shared_ptr<const std::regex> DATE_RANGE_REGEX;

      // Can either be single time
      std::shared_ptr<const RegexString> m_date;
      // Or a range of Dates
      std::shared_ptr<const Date> m_lower;
      std::shared_ptr<const Date> m_upper;

    public:
      explicit Date(const std::string& s);
      Date(const std::shared_ptr<Date>& lower, const std::shared_ptr<Date>& upper);

      bool is_range() const;
      const std::shared_ptr<const Date>& lower() const;
      const std::shared_ptr<const Date>& upper() const;

      bool operator==(const Date& other) const;
      bool operator!=(const Date& other) const;

      std::string str() const;
      void set_string(const std::string& s);
  };
}
