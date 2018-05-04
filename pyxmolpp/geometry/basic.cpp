#include "init.h"

#include "xmol/geometry/basic.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_basic(pybind11::module& geometry) {
  geometry.def("distance", &xmol::geometry::distance, py::arg("a"),
               py::arg("b"), R"(Calculate distance between two points

:param a: first point
:param b: second point
)");
  geometry.def("distance2", &xmol::geometry::distance2, py::arg("a"),
               py::arg("b"), "Calculate distance square between two points");
  geometry.def("angle", &xmol::geometry::angle, py::arg("a"), py::arg("b"),
               py::arg("c"), "Calculate angle a-b-c");
  geometry.def("dihedral_angle", &xmol::geometry::dihedral_angle, py::arg("a"),
               py::arg("b"), py::arg("c"), py::arg("d"),
               "Calculate dihedral angle between planes (a,b,c) and (b,c,d)");
}