#include "vega/vm.h"

#include <limits>

namespace vega {
  const unsigned VM::MAX_LIMIT = std::numeric_limits<VM::limit_type>::max();

  VM::VM(const std::string& vm)
    : m_limits(VM::get_pair(vm)),
      m_multiple(vm.back() == 'n')
  {}

  const std::pair<VM::limit_type, VM::limit_type>& VM::limits() const {
    return m_limits;
  }

  bool VM::multiple() const {
    return m_multiple;
  }

  bool VM::is_single() const {
    return !m_multiple && m_limits.first == m_limits.second;
  }

  bool VM::allows_multiplicity(VM::limit_type multiplicity) const {
    if (m_multiple) {
      return multiplicity >= m_limits.first && (multiplicity % m_limits.second == 0);
    }
    else {
      return multiplicity >= m_limits.first && multiplicity <= m_limits.second;
    }
  }

  const std::regex VM::REGEX("([1-9]\\d*)-([1-9]\\d*)?n?");

  std::pair<unsigned,unsigned> VM::get_pair(const std::string& vm) {
    std::smatch vm_match;
    if (std::regex_match(vm, vm_match, REGEX)) {
      return std::make_pair(
        std::stoi(vm_match[1].str()),
        vm_match[2].str().empty() ? 1 : std::stoi(vm_match[2].str())
      );
    }
    else {
      return std::make_pair(
        std::stoi(vm),
        std::stoi(vm)
      );
    }
  }

  std::ostream& operator<<(std::ostream& os, const VM& vm) {
    if (vm.m_multiple) {
      os << vm.m_limits.first << "-";
      if (vm.m_limits.second != 1) os << vm.m_limits.second;
      os << 'n';
    }
    else {
      os << vm.m_limits.first;
      if (vm.m_limits.second != vm.m_limits.first) os << '-' << vm.m_limits.second;
    }
    return os;
  }
}
