#pragma once

#include <fstream>
#include <memory>

#include "vega/dicom/reader.h"

namespace vega {
  namespace dicom {
    class Writer;

    class Preamble {
      private:
        bool m_present;

      public:
        Preamble();
        explicit Preamble(Reader& reader);

        bool present() const;

        size_t write(std::shared_ptr<Writer> writer);
    };
  }
}
