#pragma once

#include <type_traits>
#include <sstream>
#include <cassert>

#include "vega.h"
#include "vega/string.h"
#include "vega/formatter.h"

namespace vega {
  class Json {
    private:
      template <typename T>
      class HasMemberSize {
        private:
          struct Fallback { int size; };
          struct Derived : T, Fallback { };

          template <typename U>
          static std::false_type test(decltype(U::size)*);
          template <typename U>
          static std::true_type test(...);

        public:
          using type = decltype(test<Derived>(nullptr));
          static constexpr bool value = type::value;
      };

      template <typename T>
      struct has_member_size
        : public std::integral_constant<bool, HasMemberSize<T>::value>
      {
      };

    public:
      template <typename T, typename std::enable_if<has_member_size<T>::value, T>::type* = nullptr>
      static void to_json(Formatter& formatter, const T& manipulator) {
        switch (manipulator.size()) {
          case 0:
            formatter << "null";
            break;
          case 1:
            value_to_json(formatter, manipulator[0]);
            break;
          default:
            formatter << '[';
            for (size_t i = 0; i < manipulator.size(); ++i) {
              value_to_json(formatter, manipulator[i]);
              if (i < manipulator.size() - 1) formatter << ',';
            }
            formatter << ']';
        }
      }

      template <typename T, typename std::enable_if<!has_member_size<T>::value, T>::type* = nullptr>
      static void to_json(Formatter& formatter, const T& manipulator) {
        value_to_json(formatter, manipulator.value());
      }

      template <typename T>
      static T value_from_json(std::stringstream& json_string) {
        T value;
        // Surrounded in quotes
        if (json_string.peek() == '"') {
          char c;
          json_string >> c;
          assert(c == '"');
          json_string >> value;
          json_string >> c;
          assert(c == '"');
        }
        // No quotes
        else {
          json_string >> value;
        }

        return value;
      }

      template <typename T, typename V>
      static void from_json(std::stringstream& json_string, T& manipulator) {
        char c = json_string.peek();

        switch(c) {
          case 'n':
            // Should be nullptr, ending in r
            json_string >> c;
            json_string >> c;
            json_string >> c;
            json_string >> c;
            // Nothing to read into manipulator
            break;

          case '"':
            // Single string
            manipulator.emplace_back(Json::value_from_json<V>(json_string));
            break;

          case '[':
            // Start of list of elements
            json_string >> c; // Read in the [ character

            do {
              manipulator.emplace_back(Json::value_from_json<V>(json_string));
              json_string >> c;
            } while(c == ',');

            if (c != ']') throw vega::Exception("Invalid JSON: No ending ] in list");
            break;

          default:
            // Otherwise, probably a single element
            manipulator.emplace_back(Json::value_from_json<V>(json_string));
        }
      }

    private:
      template <typename T>
      static void value_to_json(Formatter& formatter, const T& value) {
        formatter << vega::to_json(value);
      }
  };

  template <>
  std::string Json::value_from_json<std::string>(std::stringstream& json_string);
}
