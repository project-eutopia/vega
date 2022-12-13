//compile with clang++ --std=c++11 -lvega -lz edit.cpp -o edit
#include <iostream>
#include <cstdlib>
#include <string>

#include "vega/dictionary/dictionary.h"
#include "vega/dicom/file.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: edit input_file output_file" << std::endl;
    return 1;
  }
  // Optional you can set the dictionary file
  // vega::dictionary::Dictionary::set_dictionary("/path/to/dictionary/dictionary.txt");
  vega::dicom::File file(argv[1]);

  auto element = file.data_set->element<vega::dictionary::PatientName>();
  if (element) {
    // If there are patient names, set them all to "Smith^Alice"
    for (auto& name : element->manipulator()) {
      name = "Smith^Alice";
    }
  } else {
    std::cout << "Coudn't find patient name, creating it" << std::endl;
    // If there are no patient names, add one with value "Smith^Alice"
    element = file.data_set()->new_element<vega::dictionary::PatientName>();
    element->manipulator()->push_back("Smith^Alice");
  }
  file.write(argv[2]);
  return 0;
} 
