#pragma once

#include <string>

namespace vega {
  class Pathname {
    public:
      Pathname() noexcept;
      Pathname(const char* s) noexcept;
      Pathname(const std::string& full_name) noexcept;

      const std::string& full_name() const;
      operator std::string() const;

      std::string extension() const;
      std::string base_name() const;
      std::string folder() const;

      friend std::istream& operator>>(std::istream& is, Pathname& pathname);

    private:
      std::string full_name_;
      size_t last_slash_;
      size_t last_dot_;

      void precompute();
  };

  const Pathname operator+(const Pathname& lhs, const Pathname& rhs);
}
