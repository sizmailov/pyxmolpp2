#include "init.h"

#include "xmol/geometry/basic.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_basic(pybind11::module& geometry) {
  geometry.def("distance",&xmol::geometry::distance);
  geometry.def("distance2",&xmol::geometry::distance2);
  geometry.def("angle",&xmol::geometry::angle);
  geometry.def("dihedral_angle",&xmol::geometry::dihedral_angle);
}