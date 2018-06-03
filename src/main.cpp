#include <string>
#include <memory>

#include "vega/dictionary/dictionary.h"
#include "vega/controller.h"

int main(int argc, char* argv[]) {
  vega::dictionary::Dictionary::set_dictionary("/usr/local/share/vega/dictionary.txt");
  vega::Controller controller(argc, argv);
  controller.run();
}
