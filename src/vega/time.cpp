#include "vega/time.h"
#include "vega/vega.h"

#include <sstream>

#include "vega/regex_string.h"

namespace vega {
  const std::shared_ptr<const std::regex> Time::SINGLE_TIME_REGEX = std::make_shared<const std::regex>(
    "([0-1]\\d|2[0-3])(\\:?[0-5]\\d(\\:?[0-5]\\d(\\.\\d{6})?)?)?"
  );
  const std::shared_ptr<const std::regex> Time::TIME_RANGE_REGEX = std::make_shared<const std::regex>(
    "\\s*([\\.\\d]*)\\s*-\\s*([\\.\\d]*)\\s*"
  );

  Time::Time(const std::string& s) {
    this->set_string(s);
  }

  void Time::set_string(const std::string& s) {
    std::smatch match;

    // Is Time range
    if (std::regex_search(s.begin(), s.end(), match, *Time::TIME_RANGE_REGEX)) {
      std::string lower = match[1].str();
      if (lower.size() > 0) {
        m_lower = std::make_shared<const Time>(lower);
      }

      std::string upper = match[2].str();
      if (upper.size() > 0) {
        m_upper = std::make_shared<const Time>(upper);
      }
    }
    // Is single Time
    else {
      m_time = std::make_shared<const RegexString>(s, Time::SINGLE_TIME_REGEX);
    }
  }

  Time::Time(const std::shared_ptr<Time>& lower, const std::shared_ptr<Time>& upper)
    :
      m_lower(std::move(lower)),
      m_upper(std::move(upper))
  {
  }

  bool Time::is_range() const { return m_lower || m_upper; }
  const std::shared_ptr<const Time>& Time::lower() const { return m_lower; }
  const std::shared_ptr<const Time>& Time::upper() const { return m_upper; }

  bool Time::operator==(const Time& other) const {
    return this->str() == other.str();
  }

  bool Time::operator!=(const Time& other) const {
    return !(*this == other);
  }

  std::string Time::str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  std::ostream& operator<<(std::ostream& os, const Time& time) {
    if (time.is_range()) {
      if (time.m_lower) os << time.m_lower->str();
      os << '-';
      if (time.m_upper) os << time.m_upper->str();
    }
    else {
      os << time.m_time->str();
    }

    return os;
  }
}
