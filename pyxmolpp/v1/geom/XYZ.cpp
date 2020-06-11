#include "XYZ.h"
#include "xmol/geom/AngleValue.h"
#include <pybind11/eigen.h>
#include <pybind11/operators.h>

namespace py = pybind11;
using namespace xmol::geom;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::XYZ>& pyXYZ) {
  pyXYZ.def(py::init<>(), "Equivalent to XYZ(0,0,0)")
      .def(py::init<double, double, double>(), py::arg("x"), py::arg("y"), py::arg("z"), "Initialize with x, y, z")
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(-py::self)
      .def(py::self / double())
      .def(py::self * double())
      .def(double() * py::self)
      .def("dot", &XYZ::dot, py::arg("rhs"), "Returns dot product")
      .def("cross", &XYZ::cross, py::arg("rhs"), "Returns cross product")
      .def("__repr__",
           [](XYZ& xyz) {
             return "[" + std::to_string(xyz.x()) + ", " + std::to_string(xyz.y()) + ", " + std::to_string(xyz.z()) +
                    "]";
           })
      .def("__str__",
           [](XYZ& xyz) {
             return "[" + std::to_string(xyz.x()) + ", " + std::to_string(xyz.y()) + ", " + std::to_string(xyz.z()) +
                    "]";
           })
      .def_property("x", &XYZ::x, &XYZ::set_x, "x coordinate")
      .def_property("y", &XYZ::y, &XYZ::set_y, "y coordinate")
      .def_property("z", &XYZ::z, &XYZ::set_z, "z coordinate")
      .def("len", &XYZ::len, "Returns vector length")
      .def("len2", &XYZ::len2, "Returns vector length squared")
      .def_property(
          "values", [](XYZ& self) { return self._eigen(); }, [](XYZ& self, const XYZ::Vector_t& v) { self = XYZ(v); },
          "Convert to/from numpy.array[float, 3]")
      .def(
          "distance", [](const XYZ& lhs, const XYZ& rhs) { return lhs.distance(rhs); }, py::arg("r"))
      .def(
          "distance2", [](const XYZ& lhs, const XYZ& rhs) { return lhs.distance2(rhs); }, py::arg("r"))
      .def(
          "angle", [](const XYZ& lhs, const XYZ& rhs) { return lhs.angle(rhs); }, py::arg("r"))
      .def(
          "angle", [](const XYZ& a, const XYZ& b, const XYZ& c) { return b.angle_between(a, c); }, py::arg("b"),
          py::arg("c"))
      .def(
          "dihedral",
          [](const XYZ& a, const XYZ& b, const XYZ& c, const XYZ& d) { return dihedral_angle(a, b, c, d); },
          py::arg("b"), py::arg("c"), py::arg("d"));
}
