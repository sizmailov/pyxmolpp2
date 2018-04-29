#include "init.h"

#include "xmol/geometry/exceptions.h"

void pyxmolpp::geometry::init_exceptions(pybind11::module& geometry) {
  auto base = py::register_exception<xmol::geometry::GeometryException>(geometry, "GeometryException");
  py::register_exception<xmol::geometry::BadRotationMatrix>(geometry, "BadRotationMatrix", base.ptr());
  py::register_exception<xmol::geometry::AlignmentError>(geometry, "AlignmentError", base.ptr());
}
