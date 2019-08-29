#include "init.h"

void pyxmolpp::init_amber(pybind11::module& m) {
  amber::init_NetCDFTrajectoryFile(m);
}
