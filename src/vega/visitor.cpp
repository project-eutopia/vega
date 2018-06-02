#include "vega/visitor.h"

#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/file.h"

namespace vega {
  Visitor::Visitor(Visitor::visitor_function_type f)
    : m_f(f)
  {}

  bool Visitor::visit(dicom::DataElement& data_element) const {
    if (m_f(data_element)) return true;

    if (data_element.is_sequence()) {
      for (auto& data_set : data_element) {
        visit(*data_set);
      }
    }

    return false;
  }

  void Visitor::visit(dicom::DataSet& data_set) const {
    auto it = data_set.begin();

    while (it != data_set.end()) {
      if (visit(**it)) {
        it = data_set.erase(it);
      }
      else {
        ++it;
      }
    }
  }

  void Visitor::visit(dicom::File& file) const {
    visit(*file.data_set());
  }

  CVisitor::CVisitor(CVisitor::visitor_function_type f)
    : m_f(f)
  {}

  void CVisitor::visit(const dicom::DataElement& data_element) const {
    m_f(data_element);

    if (data_element.is_sequence()) {
      for (const auto& data_set : data_element) {
        visit(*data_set);
      }
    }
  }

  void CVisitor::visit(const dicom::DataSet& data_set) const {
    for (const auto& data_element : data_set) {
      visit(*data_element);
    }
  }

  void CVisitor::visit(const dicom::File& file) const {
    visit(*file.data_set());
  }
}
