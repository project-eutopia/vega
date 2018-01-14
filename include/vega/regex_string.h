#pragma once

#include <string>
#include <regex>
#include <memory>

namespace vega {
  class RegexString {
    private:
      const std::shared_ptr<const std::regex> m_regex;
      std::string m_str;

    public:
      RegexString(const std::shared_ptr<const std::regex>& regex);
      RegexString(const std::string& str, const std::shared_ptr<const std::regex>& regex);

      const std::string& str() const;
      void set_str(const std::string& str);

    private:
      void validate_regex() const;
  };
}
