#pragma once

#include "vega/types.h"
#include <stdint.h>
#include <string>
#include <sstream>

namespace vega {
  // Should overload with different arguments for different types
  template <typename T>
  std::string to_string(const T& t);

  template<>
  std::string to_string<std::string>(const std::string& s);
  template<>
  std::string to_string<float>(const float& f);
  template<>
  std::string to_string<double>(const double& d);
  template<>
  std::string to_string<int8_t>(const int8_t& i);
  template<>
  std::string to_string<int16_t>(const int16_t& i);
  template<>
  std::string to_string<int32_t>(const int32_t& i);
  template<>
  std::string to_string<int64_t>(const int64_t& i);
  template<>
  std::string to_string<uint8_t>(const uint8_t& u);
  template<>
  std::string to_string<uint16_t>(const uint16_t& u);
  template<>
  std::string to_string<uint32_t>(const uint32_t& u);
  template<>
  std::string to_string<uint64_t>(const uint64_t& u);
  template<>
  std::string to_string<Byte>(const Byte& byte);
  template<>
  std::string to_string<Word>(const Word& word);
  template<>
  std::string to_string<Long>(const Long& l);

  template <typename T>
  T from_string(const std::string& s);

  template<>
  std::string from_string(const std::string& s);
  template<>
  float from_string(const std::string& s);
  template<>
  double from_string(const std::string& s);
  template<>
  int8_t from_string(const std::string& s);
  template<>
  int16_t from_string(const std::string& s);
  template<>
  int32_t from_string(const std::string& s);
  template<>
  int64_t from_string(const std::string& s);
  template<>
  uint8_t from_string(const std::string& s);
  template<>
  uint16_t from_string(const std::string& s);
  template<>
  uint32_t from_string(const std::string& s);
  template<>
  uint64_t from_string(const std::string& s);
  template<>
  Byte from_string(const std::string& s);
  template<>
  Word from_string(const std::string& s);
  template<>
  Long from_string(const std::string& s);

  template <typename IS, typename T>
  IS& to_json(IS& is, const T& t) {
    is << '"' << t << '"';
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const float& f) {
    is << f;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const double& d) {
    is << d;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const int8_t& i) {
    is << (int)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const int16_t& i) {
    is << (int)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const int32_t& i) {
    is << (int)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const int64_t& i) {
    is << (long)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const uint8_t& i) {
    is << (unsigned)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const uint16_t& i) {
    is << (unsigned)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const uint32_t& i) {
    is << (unsigned)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const uint64_t& i) {
    is << (unsigned long)i;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const Byte& byte) {
    is << (unsigned)byte.u;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const Word& word) {
    is << (unsigned)word.u;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const Long& l) {
    is << (unsigned)l.u;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const DecimalString& ds) {
    is << ds;
    return is;
  }

  template<typename IS>
  IS& to_json(IS& is, const std::string& s) {
    is << '"';

    char c;
    for (size_t i = 0; i < s.size(); ++i) {
      c = s[i];
      switch(c) {
        case '\b':
          is << '\\' << 'b';
          break;
        case '\f':
          is << '\\' << 'f';
          break;
        case '\n':
          is << '\\' << 'n';
          break;
        case '\r':
          is << '\\' << 'r';
          break;
        case '\t':
          is << '\\' << 't';
          break;
        case '"':
          is << '\\' << '"';
          break;
        case '\\':
          is << '\\' << '\\';
          break;
        default:
          is << c;
      }
    }

    is << '"';
    return is;
  }

  // Should overload with different arguments for different types
  template <typename T>
  std::string to_json(const T& t) {
    std::stringstream ss;
    to_json(ss, t);
    return ss.str();
  }
}

#include "vega/string_impl.h"
