#include "vega/controller.h"

#include "vega/dicom/file.h"
#include "vega/anonymizer.h"
#include "vega/undefined_length_remover.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <stdio.h>

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

    if ((parser_({"--anonymize"}) >> patient_id_) || parser_["anonymize"]) {
      operations_.push_back(Operation::ANONYMIZE);
    }

    if (!(parser_({"--suffix"}) >> suffix_)) {
      suffix_ = "vega";
    }

    if (!(parser_({"--folder"}) >> folder_)) {
      folder_ = "./";
    }
  }

  void Controller::run() const {
    if (input_file_.empty()) {
      // Skip if no piped data
      if (isatty(fileno(stdin))) return;

      // Read DICOM file from STDIN (piped input)
      auto ss = std::make_shared<std::stringstream>();
      *ss << std::cin.rdbuf();

      // Here we have a valid DICOM file input
      try {
        dicom::File file{ss};
        run(file, output_file_);
      }
      catch (vega::Exception& ex) {
        // Here we are piping in a list of files, so expect the "suffix" command for output
        std::string input_file;
        std::string output_file;

        std::stringstream ss2(ss->str());

        while(std::getline(ss2, input_file, '\n')) {
          if (Pathname(input_file).extension() == "dcm") {
            output_file = output_file_name_for(input_file);

            dicom::File file{input_file};
            run(file, output_file);
          }
        }
      }
    }
    else {
      // Read from filename(s)
      std::istringstream input_ss(input_file_);
      std::istringstream output_ss(output_file_);
      std::string input_file;
      std::string output_file;

      // Split by commas, and run on each such file
      while(std::getline(input_ss, input_file, ',')) {
        // No suffix, so expect comma list of output files
        if (suffix_.empty()) {
          std::getline(output_ss, output_file, ',');
        }
        else {
          output_file = output_file_name_for(input_file);
        }

        dicom::File file{input_file};
        run(file, output_file);
      }
    }
  }

  void Controller::run(dicom::File& input_file, const std::string& output_file) const {
    for (const auto& operation : operations_) {
      switch (operation) {
        case Operation::REMOVE_UNDEFINED_LENGTHS:
          {
            UndefinedLengthRemover remover{};
            remover.remove_undefined_lengths(input_file);
          }
          break;

        case Operation::ANONYMIZE:
          {
            Anonymizer anonymizer;
            anonymizer.set_patient_id(patient_id_);
            anonymizer.anonymize(input_file);
          }
          break;
      }
    }

    std::shared_ptr<std::ostream> output;

    if (output_file.empty()) {
      // Write file to STDOUT (assume in DICOM format)
      input_file.write();
    }
    else {
      // Write to file
      // Determine which format (DICOM, JSON, TXT)

      std::string lowercase_output;
      std::transform(output_file.begin(), output_file.end(), lowercase_output.begin(), ::tolower);

      // DICOM format to STDOUT
      if (output_file == "dcm" || output_file == "dicom") {
        input_file.write();
      }
      else if (output_file == "json") {
        input_file.write_json();
      }
      else if (output_file == "txt" || output_file == "text") {
        Formatter f(std::cout);
        input_file.data_set()->log(f);
      }
      else {
        // Check output file format
        std::string dcm_ending(".dcm");
        std::string json_ending(".json");
        std::string txt_ending(".txt");

        if (std::equal(dcm_ending.rbegin(), dcm_ending.rend(), output_file.rbegin())) {
          input_file.write(output_file);
        }
        else if (std::equal(json_ending.rbegin(), json_ending.rend(), output_file.rbegin())) {
          input_file.write_json(output_file);
        }
        else if (std::equal(txt_ending.rbegin(), txt_ending.rend(), output_file.rbegin())) {
          std::ofstream ofs(output_file);
          Formatter f(ofs);
          input_file.data_set()->log(f);
        }
        else {
          throw std::runtime_error("Unknown output file format");
        }
      }
    }
  }

  Pathname Controller::output_file_name_for(const Pathname& input_file) const {
    return folder_ + "/" + input_file.base_name() + "." + suffix_ + "." + input_file.extension();
  }
}
