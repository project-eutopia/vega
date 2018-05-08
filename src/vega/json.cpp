#include <iostream>
#include "vega/json.h"

namespace vega {
  template <>
  std::string Json::value_from_json<std::string>(std::stringstream& json_string) {
    std::ios::fmtflags f( json_string.flags() );

    // Whitespace is now significant
    json_string >> std::noskipws;

    char c;
    // Skip whitespace leading up to quote
    while (std::isspace(json_string.peek())) json_string >> c;
    json_string >> c;
    assert(c == '"');

    std::stringstream ss;

    while (!json_string.eof()) {
      json_string >> c;
      // Found end of string, so finish
      if (c == '"') break;

      // Process JSON string escape character
      if (c == '\\') {
        json_string >> c;
        switch (c) {
          case 'b':
            // Backspace
            ss << '\b';
            break;
          case 'f':
            // Form feed
            ss << '\f';
            break;
          case 'n':
            // New line
            ss << '\n';
            break;
          case 'r':
            // Carriage return
            ss << '\r';
            break;
          case 't':
            // Tab
            ss << '\t';
            break;
          case '"':
            // Double quote character
            ss << '"';
            break;
          case '\\':
            // Backslash
            ss << '\\';
            break;
        }
      }
      else {
        ss << c;
      }
    }

    ss.flags(f);
    return ss.str();
  }
}
