#include "vega/pathname.h"

namespace vega {
  Pathname::Pathname() noexcept
    :
      full_name_(),
      last_slash_(std::string::npos),
      last_dot_(std::string::npos)
  {}

  Pathname::Pathname(const std::string& full_name) noexcept
    :
      full_name_(full_name),
      last_slash_(std::string::npos),
      last_dot_(std::string::npos)
  {
    precompute();
  }

  Pathname::Pathname(const char* s) noexcept
    :
      full_name_(s),
      last_slash_(std::string::npos),
      last_dot_(std::string::npos)
  {
    precompute();
  }

  void Pathname::precompute() {
    last_slash_ = full_name_.rfind('/');
    last_dot_ = full_name_.rfind('.');

    if (last_dot_ != std::string::npos) {
      if (last_slash_ == std::string::npos) {
        // If no folder in front, then leading dot is not preceding file name extension
        if (last_dot_ == 0) last_dot_ = std::string::npos;
      }
      else {
        if (last_slash_ + 1 >= last_dot_) last_dot_ = std::string::npos;
      }
    }
  }

  const std::string& Pathname::full_name() const { return full_name_; }
  Pathname::operator std::string() const { return full_name_; }

  std::string Pathname::extension() const {
    return last_dot_ == std::string::npos ? "" : full_name_.substr(last_dot_ + 1);
  }

  std::string Pathname::base_name() const {
    size_t start = last_slash_ == std::string::npos ? 0 : last_slash_ + 1;

    if (last_dot_ == std::string::npos) {
      return full_name_.substr(start);
    }
    else {
      return full_name_.substr(start, last_dot_-start);
    }
  }

  std::string Pathname::folder() const {
    return last_slash_ == std::string::npos ? "./" : full_name_.substr(0, last_slash_ + 1);
  }

  std::istream& operator>>(std::istream& is, Pathname& pathname) {
    is >> pathname.full_name_;
    pathname.precompute();
    return is;
  }

  const Pathname operator+(const Pathname& lhs, const Pathname& rhs) {
    return Pathname(lhs.full_name() + rhs.full_name());
  }
}
