#pragma once

#include <random>
#include <sstream>

namespace vega {
  class Randomizer {
    private:
      std::mt19937 gen_;

    public:
      Randomizer();

      template <typename T>
      typename std::enable_if<std::is_integral<T>::value, const T>::type generate() {
        std::uniform_int_distribution<T> distribution;
        return distribution(gen_);
      }

      template <typename T>
      typename std::enable_if<std::is_floating_point<T>::value, const T>::type generate() {
        std::uniform_real_distribution<T> distribution;
        return distribution(gen_);
      }

      template <typename T>
      typename std::enable_if<std::is_same<T, std::string>::value, const T>::type generate() {
        std::stringstream ss;

        const size_t number_of_characters = 4 + generate<size_t>() % 8;

        for (size_t i = 0; i < number_of_characters; ++i) {
          ss << char('a' + char(generate<unsigned>() % 26));
        }

        return ss.str();
      }
  };
}
