#include "vega/visitor.h"

#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/file.h"

namespace vega {
  Visitor::Visitor(Visitor::data_element_visitor_type de, Visitor::data_set_visitor_type ds)
    : m_de(de), m_ds(ds)
  {}

  void Visitor::visit(dicom::DataElement& data_element) const {
    if (data_element.is_sequence()) {
      for (auto& data_set : data_element) {
        visit(*data_set);
      }
    }

    m_de(data_element);
  }

  void Visitor::visit(dicom::DataSet& data_set) const {
    for (auto& data_element : data_set) {
      visit(*data_element);
    }

    m_ds(data_set);
  }

  void Visitor::visit(dicom::File& file) const {
    visit(*file.data_set());
  }

  CVisitor::CVisitor(CVisitor::data_element_visitor_type de, CVisitor::data_set_visitor_type ds)
    : m_de(de), m_ds(ds)
  {}

  void CVisitor::visit(const dicom::DataElement& data_element) const {
    if (data_element.is_sequence()) {
      for (const auto& data_set : data_element) {
        visit(*data_set);
      }
    }

    m_de(data_element);
  }

  void CVisitor::visit(const dicom::DataSet& data_set) const {
    for (const auto& data_element : data_set) {
      visit(*data_element);
    }

    m_ds(data_set);
  }

  void CVisitor::visit(const dicom::File& file) const {
    visit(*file.data_set());
  }
}
