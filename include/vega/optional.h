#pragma once

#include "vega/vega.h"

namespace vega {
  template <typename T>
  class Optional {
    private:
      bool m_has;
      T m_value;

    public:
      Optional()
        : m_has(false), m_value()
      {}

      explicit Optional(const T& t)
        : m_has(true), m_value(t)
      {}

      explicit Optional(T&& t)
        : m_has(true), m_value(std::move(t))
      {}

      bool has_value() const { return m_has; }
      const T& value() const {
        if (!m_has) throw vega::Exception("Trying to access optional without a value");
        return m_value;
      }

      // Conversion to boolean
      operator bool() const { return m_has; }

      void set_empty() { m_has = false; }
      void set_value(const T& t) { m_has = true; m_value = t; }
      void set_value(T&& t) { m_has = true; m_value = std::move(t); }
  };
}
