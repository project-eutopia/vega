#pragma once

namespace vega {
  namespace dicom {
    class File;
    class DataSet;
    class DataElement;
  }

  class UndefinedLengthRemover {
    public:
      UndefinedLengthRemover();

      void remove_undefined_lengths(dicom::File& file) const;
      void remove_undefined_lengths(dicom::DataSet& data_set) const;

    private:
      bool remove_undefined_lengths(dicom::DataElement& data_element) const;
  };
}
