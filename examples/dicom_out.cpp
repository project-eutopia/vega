// clang++ --std=c++11 -lvega -lz dicom_out.cpp -o dicom_out

#include <cstdlib>
#include <string>

#include "vega/dictionary/dictionary.h"
#include "vega/dicom/file.h"

int main(int argc, char *argv[]) {
  // Set the dictionary file
  vega::dictionary::Dictionary::set_dictionary(std::getenv("VEGA_DICTIONARY"));

  if (argc < 3) return -1;

  // Read the DICOM file in
  const std::string file_name = argv[1];
  vega::dicom::File file(file_name);

  // Print a human-friendly representation of the file to the file
  std::ofstream output;
  output.open(argv[2], std::ofstream::out);

  vega::Formatter formatter(output);
  file.data_set()->log(formatter);

  output.close();
}
