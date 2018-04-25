#include "init.h"

#include "xmol/geometry/XYZ.h"

#include <pybind11/operators.h>


using namespace xmol::geometry;

void pyxmolpp::geometry::init_XYZ(pybind11::module& geometry) {

  py::class_<XYZ>(geometry,"XYZ")
      .def(py::init<double,double,double>(),py::arg("x"),py::arg("y"),py::arg("z"))
      .def(py::init<>())
      .def(py::self+py::self)
      .def(py::self-py::self)
      .def(-py::self)
      .def(py::self/double())
      .def(py::self*double())
      .def(double()*py::self)
      .def(double()*py::self)
      .def("dot",&XYZ::dot, py::arg("rhs"), "Returns dot product")
      .def("cross",&XYZ::cross, py::arg("rhs"), "Returns cross product")
      .def_property("x",&XYZ::x,&XYZ::set_x)
      .def_property("y",&XYZ::y,&XYZ::set_y)
      .def_property("z",&XYZ::z,&XYZ::set_z)
      .def("len",&XYZ::len, "Returns vector length")
      .def("len2",&XYZ::len2, "Returns vector length squared")
      ;

}