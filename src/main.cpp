#include <string>
#include <memory>

#include "vega/dictionary/dictionary.h"
#include "vega/controller.h"

int main(int argc, char* argv[]) {
  vega::dictionary::Dictionary::set_dictionary(std::getenv("VEGA_DICTIONARY"));
  vega::Controller controller(argc, argv);
  controller.run();
}
