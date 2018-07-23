#include <string>
#include <memory>

#include "vega/dictionary/dictionary.h"
#include "vega/controller.h"

int main(int argc, char* argv[]) {
  vega::Controller controller(argc, argv);
  controller.run();
}
