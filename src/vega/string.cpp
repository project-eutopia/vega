#include "vega/string.h"
#include "vega/math.h"

#include <sstream>
#include <iomanip>

namespace vega {
  template<>
  std::string to_string(const std::string& s) { return s; }
  template<>
  std::string to_string(const float& f) { std::stringstream ss; ss << std::scientific << f; return ss.str(); }
  template<>
  std::string to_string(const double& d) {
    int exponent;
    double mantissa = math::frexp10(d, &exponent);

    std::stringstream ss;
    ss << mantissa << "e" << exponent;
    return ss.str();
  }

  template<>
  std::string to_string(const int8_t& i) { return std::to_string(i); }
  template<>
  std::string to_string(const int16_t& i) { return std::to_string(i); }
  template<>
  std::string to_string(const int32_t& i) { return std::to_string(i); }
  template<>
  std::string to_string(const int64_t& i) { return std::to_string(i); }
  template<>
  std::string to_string(const uint8_t& u) { return std::to_string(u); }
  template<>
  std::string to_string(const uint16_t& u) { return std::to_string(u); }
  template<>
  std::string to_string(const uint32_t& u) { return std::to_string(u); }
  template<>
  std::string to_string(const uint64_t& u) { return std::to_string(u); }

  template<>
  std::string to_string(const Byte& byte) {
    std::ostringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned>(byte.u);
    return ss.str();
  }

  template<>
  std::string to_string(const Word& word) {
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << static_cast<unsigned>(word.u);
    return ss.str();
  }

  template<>
  std::string to_string(const Long& l) {
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << static_cast<unsigned>(l.u);
    return ss.str();
  }

  template<>
  std::string from_string(const std::string& s) {
    return s;
  }

  template<>
  float from_string(const std::string& s) {
    return std::stof(s);
  }

  template<>
  double from_string(const std::string& s) {
    return std::stod(s);
  }

  template<>
  int8_t from_string(const std::string& s) {
    return std::stoi(s);
  }

  template<>
  int16_t from_string(const std::string& s) {
    return std::stoi(s);
  }

  template<>
  int32_t from_string(const std::string& s) {
    return std::stol(s);
  }

  template<>
  int64_t from_string(const std::string& s) {
    return std::stoll(s);
  }

  template<>
  uint8_t from_string(const std::string& s) {
    return std::stoul(s);
  }

  template<>
  uint16_t from_string(const std::string& s) {
    return std::stoul(s);
  }

  template<>
  uint32_t from_string(const std::string& s) {
    return std::stoul(s);
  }

  template<>
  uint64_t from_string(const std::string& s) {
    return std::stoull(s);
  }

  template<>
  Byte from_string(const std::string& s) {
    return Byte {.u = (uint8_t)std::stoul(s.substr(2,2), nullptr, 16)};
  }

  template<>
  Word from_string(const std::string& s) {
    return Word {.u = (uint16_t)std::stoul(s.substr(2,4), nullptr, 16)};
  }

  template<>
  Long from_string(const std::string& s) {
    return Long {.u = (uint32_t)std::stoul(s.substr(2,8), nullptr, 16)};
  }

  template<>
  std::string to_json(const std::string& s) {
    std::stringstream ss;
    ss << '"' << s << '"';
    return ss.str();
  }

  template<>
  std::string to_json<float>(const float& f) {
    std::stringstream ss;
    ss << f;
    return ss.str();
  }

  template<>
  std::string to_json<double>(const double& d) {
    std::stringstream ss;
    ss << d;
    return ss.str();
  }

  template<>
  std::string to_json<int8_t>(const int8_t& i) {
    std::stringstream ss;
    ss << (int)i;
    return ss.str();
  }

  template<>
  std::string to_json<int16_t>(const int16_t& i) {
    std::stringstream ss;
    ss << (int)i;
    return ss.str();
  }

  template<>
  std::string to_json<int32_t>(const int32_t& i) {
    std::stringstream ss;
    ss << (int)i;
    return ss.str();
  }

  template<>
  std::string to_json<int64_t>(const int64_t& i) {
    std::stringstream ss;
    ss << (long)i;
    return ss.str();
  }

  template<>
  std::string to_json<uint8_t>(const uint8_t& u) {
    std::stringstream ss;
    ss << (unsigned)u;
    return ss.str();
  }

  template<>
  std::string to_json<uint16_t>(const uint16_t& u) {
    std::stringstream ss;
    ss << (unsigned)u;
    return ss.str();
  }

  template<>
  std::string to_json<uint32_t>(const uint32_t& u) {
    std::stringstream ss;
    ss << (unsigned)u;
    return ss.str();
  }

  template<>
  std::string to_json<uint64_t>(const uint64_t& u) {
    std::stringstream ss;
    ss << (unsigned long)u;
    return ss.str();
  }

  template<>
  std::string to_json(const Byte& byte) {
    std::stringstream ss;
    ss << (unsigned)byte.u;
    return ss.str();
  }

  template<>
  std::string to_json(const Word& word) {
    std::stringstream ss;
    ss << (unsigned)word.u;
    return ss.str();
  }

  template<>
  std::string to_json(const Long& l) {
    std::stringstream ss;
    ss << (unsigned)l.u;
    return ss.str();
  }

  template<>
  std::string to_json(const DecimalString& ds) {
    return ds.str();
  }
}
