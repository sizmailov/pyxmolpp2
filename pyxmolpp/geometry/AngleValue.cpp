#include "init.h"

#include "xmol/geometry/AngleValue.h"

#include "pybind11/operators.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_AngleValue(pybind11::module& geometry) {

  geometry.def("degrees_to_radians", degrees_to_radians, py::arg("degrees"));
  geometry.def("radians_to_degrees", radians_to_degrees, py::arg("radians"));

  py::class_<AngleValue>(geometry, "AngleValue")
      .def(py::self + py::self, py::return_value_policy::copy)
      .def(py::self - py::self, py::return_value_policy::copy)
      .def(-py::self, py::return_value_policy::copy)
      //      .def(py::self += py::self)
      //      .def(py::self -= py::self)
      .def(py::self / double(), py::return_value_policy::copy)
      .def(py::self * double(), py::return_value_policy::copy)
      .def(double() * py::self, py::return_value_policy::copy)
      .def(py::self <= py::self, py::return_value_policy::copy)
      .def(py::self >= py::self, py::return_value_policy::copy)
      .def(py::self < py::self, py::return_value_policy::copy)
      .def(py::self > py::self, py::return_value_policy::copy)
      .def_property_readonly("degrees", &AngleValue::degrees,
                             py::return_value_policy::copy,
                             "Return angle value in degrees")
      .def_property_readonly("radians", &AngleValue::radians,
                             py::return_value_policy::copy,
                             "Return angle value in radians")
      .def(
          "to_standard_range",
          &AngleValue::to_standard_range,
          py::return_value_policy::copy,
          "Return angle in range :math:`[-\\pi..\\pi)`");

  geometry.def("Degrees",
               [](double value) { return AngleValue(Degrees(value)); },
               py::return_value_policy::copy,
               py::arg("degrees"));
  geometry.def("Radians",
               [](double value) { return AngleValue(Radians(value)); },
               py::return_value_policy::copy,
               py::arg("radians"));

  geometry.def("sin", xmol::geometry::sin, py::arg("angle"), py::return_value_policy::copy);
  geometry.def("cos", xmol::geometry::cos, py::arg("angle"), py::return_value_policy::copy);
  geometry.def("tan", xmol::geometry::tan, py::arg("angle"), py::return_value_policy::copy);
  geometry.def("fabs", xmol::geometry::fabs, py::arg("angle"), py::return_value_policy::copy);
}
