#pragma once

#include <gtest/gtest.h>
#include <string>

#include "vega/dictionary/dictionary.h"

namespace tests {
  // Credit:  https://stackoverflow.com/a/29921055/2435405
  constexpr const char* const strend(const char* const str) {
      return *str ? strend(str + 1) : str;
  }

  constexpr const char* const fromlastslash(const char* const start, const char* const end) {
      return (end >= start && *end != '/' && *end != '\\') ? fromlastslash(start, end - 1) : (end + 1);
  }

  constexpr const char* const pathlast(const char* const path) {
      return fromlastslash(path, strend(path));
  }

  const std::string base_path(const char* const path) {
    std::string fullpath(path);
    std::string file(pathlast(path));
    return fullpath.substr(0, fullpath.size() - file.size());
  }

  const std::string path = base_path(__FILE__);

  const std::string path_to_file(const std::string& s) {
    return path + s;
  }
}
