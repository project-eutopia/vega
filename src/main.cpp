#include <string>
#include <memory>

#include "vega/dictionary/dictionary.h"
#include "vega/dicom/file.h"

int main() {
  vega::dictionary::Dictionary::set_dictionary("/home/chris/cpp/vega/test/dictionary.txt");
  // FIXME: do something
}
