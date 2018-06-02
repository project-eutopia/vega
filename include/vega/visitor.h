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
      using visitor_function_type = std::function<bool(dicom::DataElement&)>;

    private:
      const visitor_function_type m_f;

    public:
      // Function returns true if data element to be removed
      Visitor(visitor_function_type f);

      void visit(dicom::DataSet& data_set) const;
      void visit(dicom::File& file) const;

    private:
      bool visit(dicom::DataElement& data_element) const;
  };

  class CVisitor {
    public:
      using visitor_function_type = std::function<void(const dicom::DataElement&)>;

    private:
      const visitor_function_type m_f;

    public:
      CVisitor(visitor_function_type f);

      void visit(const dicom::DataSet& data_set) const;
      void visit(const dicom::File& file) const;

    private:
      void visit(const dicom::DataElement& data_element) const;
  };
}
