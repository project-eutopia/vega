#pragma once

#include "argh/argh.h"

namespace vega {
  class Controller {
    private:
      argh::parser parser_;

      std::string input_file_;
      std::string output_file_;

      enum class Operation {
        REMOVE_UNDEFINED_LENGTHS
      };

      std::vector<Operation> operations_;

    public:
      Controller(int argc, char* argv[]);

      void run() const;
  };
}
