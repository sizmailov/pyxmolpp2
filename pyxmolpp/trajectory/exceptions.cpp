#include "init.h"

#include "xmol/trajectory/exceptions.h"

void pyxmolpp::trajectory::init_exceptions(pybind11::module& pdb) {
  py::register_exception<xmol::trajectory::TrajectoryException>(pdb, "TrajectoryException");
}
