#include "vega/dicom/data_element_header.h"

namespace vega {
  namespace dicom {
    const DataElementHeader::length_type DataElementHeader::UNDEFINED_LENGTH = 0xFFFFFFFF;

    bool DataElementHeader::is_undefined_length() const {
      return length == DataElementHeader::UNDEFINED_LENGTH;
    }
  }
}

