#include "vega/date_time.h"
#include "vega/vega.h"

#include <sstream>

#include "vega/regex_string.h"

namespace vega {
  const std::shared_ptr<const std::regex> DateTime::SINGLE_REGEX = std::make_shared<const std::regex>(
    "\\d{4}((0[1-9]|1[0-2])((0[1-9]|[1-2]\\d|3[0-1])(([0-1]\\d|2[0-4])([0-5]\\d(([0-5]\\d|60)(\\.\\d{1,6})?)?)?)?)?)?([\\+\\-](0\\d|1[0-4])([0-5]\\d)?)?"
  );
  // TODO: separating colon '-' should not preceed a pair from 00 to 14
  // (so only years from 1500 are unambiguous).
  // So we should use negative look-ahead
  const std::shared_ptr<const std::regex> DateTime::RANGE_REGEX = std::make_shared<const std::regex>(
    "\\s*([\\+\\-\\.\\d]*)\\s*-\\s*([\\+\\-\\.\\d]*)\\s*"
  );

  DateTime::DateTime() {}

  DateTime::DateTime(const std::string& s) {
    this->set_string(s);
  }

  void DateTime::set_string(const std::string& s) {
    std::smatch match;

    // Is DateTime range
    if (std::regex_search(s.begin(), s.end(), match, *DateTime::RANGE_REGEX)) {
      std::string lower = match[1].str();
      if (lower.size() > 0) {
        m_lower = std::make_shared<const DateTime>(lower);
      }

      std::string upper = match[2].str();
      if (upper.size() > 0) {
        m_upper = std::make_shared<const DateTime>(upper);
      }
    }
    // Is single DateTime
    else {
      m_value = std::make_shared<const RegexString>(s, DateTime::SINGLE_REGEX);
    }
  }

  DateTime::DateTime(const std::shared_ptr<DateTime>& lower, const std::shared_ptr<DateTime>& upper)
    :
      m_lower(std::move(lower)),
      m_upper(std::move(upper))
  {
  }

  bool DateTime::is_range() const { return m_lower || m_upper; }
  const std::shared_ptr<const DateTime>& DateTime::lower() const { return m_lower; }
  const std::shared_ptr<const DateTime>& DateTime::upper() const { return m_upper; }

  bool DateTime::operator==(const DateTime& other) const {
    return this->str() == other.str();
  }

  bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
  }

  std::string DateTime::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  std::ostream& operator<<(std::ostream& os, const DateTime& date_time) {
    if (date_time.is_range()) {
      if (date_time.m_lower) os << date_time.m_lower->str();
      os << '-';
      if (date_time.m_upper) os << date_time.m_upper->str();
    }
    else {
      os << date_time.m_value->str();
    }

    return os;
  }

  std::string DateTime::read_single_string_from(std::istream& is) {
    std::stringstream ss;
    char c;
    bool found = false;

    while (!is.eof()) {
      c = is.peek();
      if (std::isdigit(c) || c == '.' || c == '+') {
        found = true;
        is >> c;
        ss << c;
      }
      else if (std::isspace(c)) {
        is >> c;
      }
      // Might be end, might be timezone offset
      else if (c == '-') {
        auto pos = is.tellg();
        is.seekg(0, is.end);
        auto end = is.tellg();
        is.seekg(pos, is.beg);

        // Here we don't have enough to have a timezone
        if (end - pos < 2) {
          break;
        }

        is.seekg(1, is.cur);

        c = is.peek();
        // Maybe?  Check next
        if (c == '0') {
          is.seekg(1, is.cur);
          c = is.peek();

          if (!std::isdigit(c)) {
            is.seekg(pos, is.beg);
            break;
          }
        }
        else if (c == '1') {
          is.seekg(1, is.cur);
          c = is.peek();

          if (!std::isdigit(c) || c > '4') {
            is.seekg(pos, is.beg);
            break;
          }
        }
        else {
          is.seekg(pos, is.beg);
          break;
        }

        is.seekg(pos, is.beg);
        is >> c;
        ss << c;
      }
      else {
        break;
      }
    }

    if (found) return ss.str();
    else return std::string();
  }

  std::istream& operator>>(std::istream& is, DateTime& date_time) {
    range_read(is, date_time);
    return is;
  }
}
