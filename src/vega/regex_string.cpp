#include "vega/regex_string.h"
#include "vega/vega.h"

namespace vega {

  RegexString::RegexString(const std::shared_ptr<const std::regex>& regex)
    : RegexString("", regex)
  {}

  RegexString::RegexString(const std::string& str, const std::shared_ptr<const std::regex>& regex)
    :
      m_regex(regex),
      m_str(str)
  {
    this->validate_regex();
  }

  const std::string& RegexString::str() const {
    return m_str;
  }

  void RegexString::set_str(const std::string& str) {
    m_str = str;
    this->validate_regex();
  }

  void RegexString::validate_regex() const {
    if (!std::regex_match(m_str, *m_regex)) {
      throw Exception("String \"" + m_str + "\" does not match expected pattern");
    }
  }
}
