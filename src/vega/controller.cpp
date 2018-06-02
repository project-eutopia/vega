#include "vega/controller.h"

#include "vega/dicom/file.h"
#include "vega/undefined_length_remover.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

namespace vega {
  Controller::Controller(int argc, char* argv[])
    : parser_(argc, argv)
  {
    if (!(parser_({"-i", "--input"}) >> input_file_)) {
      input_file_ = "";
    }
    if (!(parser_({"-o", "--output"}) >> output_file_)) {
      output_file_ = "";
    }

    if (parser_["remove_undefined_lengths"]) {
      operations_.push_back(Operation::REMOVE_UNDEFINED_LENGTHS);
    }
  }

  void Controller::run() const {
    std::shared_ptr<dicom::File> file;

    if (input_file_.empty()) {
      // Read DICOM file from STDIN (piped input)
      auto ss = std::make_shared<std::stringstream>();
      *ss << std::cin.rdbuf();
      file = std::make_shared<dicom::File>(ss);
    }
    else {
      // Read from filename
      file = std::make_shared<dicom::File>(input_file_);
    }

    for (const auto& operation : operations_) {
      switch (operation) {
        case Operation::REMOVE_UNDEFINED_LENGTHS:
          {
            UndefinedLengthRemover remover{};
            remover.remove_undefined_lengths(*file->data_set());
          }
          break;
      }
    }

    std::shared_ptr<std::ostream> output;

    if (output_file_.empty()) {
      // Write file to STDOUT (assume in DICOM format)
      file->write();
    }
    else {
      // Write to file
      // Determine which format (DICOM, JSON, TXT)

      std::string lowercase_output;
      std::transform(output_file_.begin(), output_file_.end(), lowercase_output.begin(), ::tolower);

      // DICOM format to STDOUT
      if (output_file_ == "dcm" || output_file_ == "dicom") {
        file->write();
      }
      else if (output_file_ == "json") {
        file->write_json();
      }
      else if (output_file_ == "txt" || output_file_ == "text") {
        Formatter f(std::cout);
        file->data_set()->log(f);
      }
      else {
        // Check output file format
        std::string dcm_ending(".dcm");
        std::string json_ending(".json");
        std::string txt_ending(".txt");

        if (std::equal(dcm_ending.rbegin(), dcm_ending.rend(), output_file_.rbegin())) {
          file->write(output_file_);
        }
        else if (std::equal(json_ending.rbegin(), json_ending.rend(), output_file_.rbegin())) {
          file->write_json(output_file_);
        }
        else if (std::equal(txt_ending.rbegin(), txt_ending.rend(), output_file_.rbegin())) {
          std::ofstream ofs(output_file_);
          Formatter f(ofs);
          file->data_set()->log(f);
        }
        else {
          throw std::runtime_error("Unknown output file format");
        }
      }
    }
  }
}
