#include "vega/dicom/data_element_header.h"

namespace vega {
  namespace dicom {
    const DataElementHeader::length_type DataElementHeader::UndefinedLength = 0xFFFFFFFF;

    bool DataElementHeader::is_undefined_length() const {
      return length == DataElementHeader::UndefinedLength;
    }
  }
}

