#pragma once

#include "vega/tag.h"
#include "vega/vr.h"
#include <stdint.h>

namespace vega {
  namespace dicom {
    /**
     * \struct DataElementHeader
     * \brief An object that stores the header information for \link DataElement DataElements\endlink.
     *
     * The header of a DICOM file DataElement includes a Tag, VR, and \link length_type length\endlink.
     */
    struct DataElementHeader {
      typedef uint32_t length_type;

      Tag tag;
      VR vr;
      length_type length;

      bool is_undefined_length() const;

      static const length_type UNDEFINED_LENGTH;
    };
  }
}
