#pragma once

#include "vega/tag.h"
#include "vega/vr.h"
#include <stdint.h>

namespace vega {
  namespace dicom {
    struct DataElementHeader {
      typedef uint32_t length_type;

      Tag tag;
      VR vr;
      length_type length;

      bool is_undefined_length() const;

      static const length_type UndefinedLength;
    };
  }
}

