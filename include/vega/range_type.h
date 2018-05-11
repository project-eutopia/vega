#pragma once

#include <istream>
#include <memory>
#include <cassert>

#include <iostream>

namespace vega {
  class RegexString;

  template <typename T>
  void range_read(std::istream& is, T& t) {
    std::ios::fmtflags f(is.flags());
    is >> std::noskipws;

    char c;
    auto s = T::read_single_string_from(is);

    // Either single, or range with start
    if (s.size() > 0) {
      // Here it is a range with start
      if (is.peek() == '-') {
        is >> c;
        t.m_value = nullptr;

        t.m_lower = std::make_shared<const T>(s);

        s = T::read_single_string_from(is);
        if (s.size() > 0) {
          t.m_upper = std::make_shared<const T>(s);
        }
        else {
          t.m_upper = nullptr;
        }
      }
      // Here it is single
      else {
        t.m_lower = nullptr;
        t.m_upper = nullptr;
        t.m_value = std::make_shared<const RegexString>(s, T::SINGLE_REGEX);
      }
    }
    // Range with just end
    else {
      is >> c;
      assert(c == '-');

      s = T::read_single_string_from(is);
      assert(s.size() > 0);

      t.m_lower = nullptr;
      t.m_upper = std::make_shared<const T>(s);
      t.m_value = nullptr;
    }

    is.flags(f);
  }
}
