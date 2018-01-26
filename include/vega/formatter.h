// Credit to:  https://stackoverflow.com/a/25217417/2435405
#include <ostream>

#pragma once

namespace vega {
  // TODO: inherit from ostream? accept std::endl in <<?
  class Formatter {
    public:

      explicit Formatter(std::ostream& os, bool active = true) : m_os(os), m_indent(0), m_active(active) {}
      void increase_indent() { ++m_indent; }
      void decrease_indent() { --m_indent; }

      Formatter& indent() {
        if (m_active) {
          for (size_t i = 0; i < m_indent; ++i) {
            m_os << "  ";
          }
        }
        return *this;
      }

      Formatter& newline() {
        if (m_active) m_os << std::endl;
        return *this;
      }

      template<typename T> friend Formatter& operator<<(Formatter&, const T&);

    private:
      std::ostream& m_os;
      size_t m_indent;
      bool m_active;
  };
};

template<typename T>
vega::Formatter& vega::operator<<(vega::Formatter& log, const T& op) {
  if (log.m_active) log.m_os << op;
  return log;
}
