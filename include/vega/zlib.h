#pragma once

#include <memory>
#include <string>
#include <sstream>

namespace vega {
  class RFC1951 {
    public:
      static std::shared_ptr<std::stringstream> inflate(const std::string& compressed);
      static std::shared_ptr<std::stringstream> deflate(const std::string& uncompressed);
  };
}
