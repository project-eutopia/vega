#pragma once

#include <vector>
#include "vega/pathname.h"
#include "vega/anonymizer.h"
#include "argh/argh.h"

namespace vega {
  namespace dicom {
    class File;
  }

  class Controller {
    private:
      argh::parser parser_;

      std::string input_file_;
      std::string output_file_;
      std::string patient_id_;
      std::string suffix_;
      Pathname folder_;

      enum class Operation {
        REMOVE_UNDEFINED_LENGTHS,
        ANONYMIZE
      };

      std::vector<Operation> operations_;

    public:
      Controller(int argc, char* argv[]);
      void run() const;

    private:
      void run(dicom::File& input_file, const std::string& output_file) const;
      Pathname output_file_name_for(const Pathname& input_file) const;
  };
}
