#pragma once

#include <functional>

namespace vega {
  namespace dicom {
    class DataElement;
    class DataSet;
    class File;
  }

  class Visitor {
    public:
      using data_element_visitor_type = std::function<void(dicom::DataElement&)>;
      using data_set_visitor_type = std::function<void(dicom::DataSet&)>;

    private:
      const data_element_visitor_type m_de;
      const data_set_visitor_type m_ds;

    public:
      Visitor(data_element_visitor_type de, data_set_visitor_type ds);

      void visit(dicom::DataElement& data_element) const;
      void visit(dicom::DataSet& data_set) const;
      void visit(dicom::File& file) const;
  };

  class CVisitor {
    public:
      using data_element_visitor_type = std::function<void(const dicom::DataElement&)>;
      using data_set_visitor_type = std::function<void(const dicom::DataSet&)>;

    private:
      const data_element_visitor_type m_de;
      const data_set_visitor_type m_ds;

    public:
      CVisitor(data_element_visitor_type de, data_set_visitor_type ds);

      void visit(const dicom::DataElement& data_element) const;
      void visit(const dicom::DataSet& data_set) const;
      void visit(const dicom::File& file) const;
  };
}
