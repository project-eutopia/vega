#include "vega/vega.h"

namespace vega {
  Endian get_machine_endian() {
	int i = 1;
	char *p = (char*) &i;

	if(p[0] == 1) {//Lowest address contains the least significant byte
      return Endian::LITTLE;
	} else {
      return Endian::BIG;
	}
  }
}
