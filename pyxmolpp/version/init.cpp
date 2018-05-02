#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_version(pybind11::module& version) {
  pyxmolpp::version::init_version(version);
}
