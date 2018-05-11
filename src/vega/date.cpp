#include "vega/date.h"
#include "vega/vega.h"

#include <sstream>
#include <cctype>

#include "vega/regex_string.h"

namespace vega {
  const std::shared_ptr<const std::regex> Date::SINGLE_REGEX = std::make_shared<const std::regex>(
    "\\d{4}(\\.?(0[1-9]|1[0-2])(\\.?(0[1-9]|[1-2]\\d|3[0-1]))?)?"
  );
  const std::shared_ptr<const std::regex> Date::RANGE_REGEX = std::make_shared<const std::regex>(
    "\\s*([\\.\\d]*)\\s*-\\s*([\\.\\d]*)\\s*"
  );

  Date::Date() {}

  Date::Date(const std::string& s) {
    this->set_string(s);
  }

  void Date::set_string(const std::string& s) {
    std::smatch match;

    m_lower = nullptr;
    m_upper = nullptr;
    m_value = nullptr;

    // Is Date range
    if (std::regex_search(s.begin(), s.end(), match, *Date::RANGE_REGEX)) {
      std::string lower = match[1].str();
      if (lower.size() > 0) {
        m_lower = std::make_shared<const Date>(lower);
      }

      std::string upper = match[2].str();
      if (upper.size() > 0) {
        m_upper = std::make_shared<const Date>(upper);
      }
    }
    // Is single Date
    else {
      m_value = std::make_shared<const RegexString>(s, Date::SINGLE_REGEX);
    }
  }

  Date::Date(const std::shared_ptr<Date>& lower, const std::shared_ptr<Date>& upper)
    :
      m_lower(std::move(lower)),
      m_upper(std::move(upper))
  {
  }

  bool Date::is_range() const { return m_lower || m_upper; }
  const std::shared_ptr<const Date>& Date::lower() const { return m_lower; }
  const std::shared_ptr<const Date>& Date::upper() const { return m_upper; }

  bool Date::operator==(const Date& other) const {
    return this->str() == other.str();
  }

  bool Date::operator!=(const Date& other) const {
    return !(*this == other);
  }

  std::string Date::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  std::ostream& operator<<(std::ostream& os, const Date& date) {
    if (date.is_range()) {
      if (date.m_lower) os << date.m_lower->str();
      os << '-';
      if (date.m_upper) os << date.m_upper->str();
    }
    else {
      os << date.m_value->str();
    }

    return os;
  }

  std::string Date::read_single_string_from(std::istream& is) {
    std::stringstream ss;
    char c;
    bool found = false;

    while (!is.eof()) {
      c = is.peek();
      if (std::isdigit(c)) {
        found = true;
        is >> c;
        ss << c;
      }
      else if (std::isspace(c) || c == '.') {
        is >> c;
      }
      else {
        break;
      }
    }

    if (found) return ss.str();
    else return std::string();
  }

  std::istream& operator>>(std::istream& is, Date& date) {
    range_read(is, date);
    return is;
  }
}
