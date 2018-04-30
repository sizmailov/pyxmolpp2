#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_trajectory(pybind11::module& trajectory) {
  pyxmolpp::trajectory::init_exceptions(trajectory);
  pyxmolpp::trajectory::init_Trajectory(trajectory);
}
