// Credit to:  https://stackoverflow.com/a/25217417/2435405

#pragma once

namespace vega {
  // TODO: inherit from ostream? accept std::endl in <<?
  class Logger {
    public:

      explicit Logger(std::ostream& os, bool active = true) : m_os(os), m_indent(0), m_active(active) {}
      void increase_indent() { ++m_indent; }
      void decrease_indent() { --m_indent; }

      Logger& indent() {
        if (m_active) {
          for (size_t i = 0; i < m_indent; ++i) {
            m_os << "  ";
          }
        }
        return *this;
      }

      Logger& newline() {
        if (m_active) m_os << std::endl;
        return *this;
      }

      template<typename T> friend Logger& operator<<(Logger&, const T&);

    private:
      std::ostream& m_os;
      size_t m_indent;
      bool m_active;
  };
};

template<typename T>
vega::Logger& vega::operator<<(vega::Logger& log, const T& op) {
  if (log.m_active) log.m_os << op;
  return log;
}
