namespace vega {
  template <typename T>
  std::string to_string(const T& t) {
    return t.str();
  }

  template <typename T>
  T from_string(const std::string& s) {
    return T{s};
  }
}
