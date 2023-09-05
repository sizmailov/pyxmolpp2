#include "XYZ.h"
#include "xmol/geom/AngleValue.h"
#include <pybind11/eigen.h>
#include <pybind11/operators.h>

namespace py = pybind11;
using namespace xmol::geom;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::XYZ>& pyXYZ) {
  pyXYZ.def(py::init<>(), "Default constructor")
      .def(py::init<double, double, double>(), py::arg("x"), py::arg("y"), py::arg("z"), "Constructor")
      .def(py::init<XYZ>(), "Copy constructor")
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(-py::self)
      .def(py::self / double())
      .def(py::self * double())
      .def(double() * py::self)
      .def("dot", &XYZ::dot, "Dot product")
      .def("cross", &XYZ::cross, "Cross product")
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
      .def("len", &XYZ::len, "Vector length")
      .def("len2", &XYZ::len2, "Vector length squared")
      .def_property(
          "values", [](XYZ& self) { return self._eigen(); }, [](XYZ& self, const XYZ::Vector_t& v) { self = XYZ(v); },
          "Convert to/from numpy.ndarray[float, 3]")
      .def(
          "distance", [](const XYZ& lhs, const XYZ& rhs) { return lhs.distance(rhs); }, py::arg("r"),
          "Distance")
      .def(
          "distance2", [](const XYZ& lhs, const XYZ& rhs) { return lhs.distance2(rhs); }, py::arg("r"),
          "Distance squared")
      .def(
          "angle", [](const XYZ& lhs, const XYZ& rhs) { return lhs.angle(rhs); }, py::arg("r"),
          "Angle between two vectors")
      .def(
          "angle", [](const XYZ& a, const XYZ& b, const XYZ& c) { return b.angle_between(a, c); }, py::arg("b"),
          py::arg("c"), "Linear angle by three points")
      .def(
          "dihedral", [](const XYZ& a, const XYZ& b, const XYZ& c, const XYZ& d) { return dihedral_angle(a, b, c, d); },
          py::arg("b"), py::arg("c"), py::arg("d"), "Dihedral angle by four points");
}
