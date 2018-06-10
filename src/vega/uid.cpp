#include "vega/uid.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <sstream>

namespace vega {
  const size_t UID::MAX_LENGTH = 64;

  // Received from: www.medicalconnections.co.uk
  const UID UID::BASE("1.2.826.0.1.3680043.9.7229");
  const UID UID::IMPLEMENTATION_CLASS_UID(UID::BASE.str() + ".1");

  UID::UID() : m_uid() {}
  UID::UID(const std::string& uid) : m_uid(uid) {}

  bool UID::operator==(const UID& other) const { return this->str() == other.str(); }
  bool UID::operator!=(const UID& other) const { return this->str() != other.str(); }
  bool UID::operator<(const UID& other)  const { return this->str() < other.str(); }

  UID UID::generate(uint16_t num) {
    // UID consists of the following 4 elements separated by periods:
    // base
    // random number
    // user input number (if supplied)
    // timestamp of form YYYYMMDDhhmmss
    // BASE.RANDOM.NUM.TIMESTAMP
    // or
    // BASE.RANDOM.TIMESTAMP
    std::string num_string = std::to_string(num);

    // Length of random number of digits
    size_t length_of_random = MAX_LENGTH - (
      UID::BASE.str().size() + 1 + // base prefix and dot
      (num == 0 ? 0 : num_string.size()+1) + // input number
      1 + 14 // dot and ending timestamp
    );

    std::ostringstream ss;
    // Base
    ss << UID::BASE.str() << '.';
    // Random part (first digit must be non-zero
    ss << (char)('1' + (char)(std::rand() % 9));
    for (size_t i = 1; i < length_of_random; ++i) {
      ss << (char)('0' + (char)(std::rand() % 10));
    }
    // User input number
    if (num > 0) {
      ss << '.' << num_string;
    }
    // Timestamp
    ss << '.' << timestamp();

    return UID(ss.str());
  }

  std::string UID::timestamp() {
    time_t t = time(NULL);
    auto utc_time = gmtime(&t);

    char buff[15];
    strftime(buff, sizeof(buff), "%Y%m%d%H%M%S", utc_time);
    return std::string(buff);
  }

  const std::string& UID::str() const { return m_uid; }

  std::ostream& operator<<(std::ostream& os, const UID& uid) {
    os << uid.str();
    return os;
  }

  std::istream& operator>>(std::istream& is, UID& uid) {
    std::stringstream ss;
    char c;

    while (!is.eof() && (std::isdigit(is.peek()) || is.peek() == '.')) {
      is >> c;
      ss << c;
    }

    uid.m_uid = ss.str();
    return is;
  }
}
