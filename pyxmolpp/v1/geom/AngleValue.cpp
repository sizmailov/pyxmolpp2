#include "AngleValue.h"

#include <pybind11/operators.h>

using namespace xmol::geom;
namespace py = pybind11;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::AngleValue>& pyAngleValue) {

  pyAngleValue.def(py::self + py::self, py::return_value_policy::copy)
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
      .def("to_standard_range", &AngleValue::to_standard_range, py::return_value_policy::copy,
           "Return angle in range :math:`[-\\pi..\\pi)`");

  //  geometry.def("sin", xmol::geometry::sin, py::arg("angle"), py::return_value_policy::copy);
  //  geometry.def("cos", xmol::geometry::cos, py::arg("angle"), py::return_value_policy::copy);
  //  geometry.def("tan", xmol::geometry::tan, py::arg("angle"), py::return_value_policy::copy);
  //  geometry.def("fabs", xmol::geometry::fabs, py::arg("angle"), py::return_value_policy::copy);
}

void pyxmolpp::v1::define_angle_free_functions(pybind11::module& module) {
  module.def("degrees_to_radians", degrees_to_radians, py::arg("degrees"));
  module.def("radians_to_degrees", radians_to_degrees, py::arg("radians"));

  module.def(
      "Degrees", [](double value) { return AngleValue(Degrees(value)); }, py::return_value_policy::copy,
      py::arg("degrees"));
  module.def(
      "Radians", [](double value) { return AngleValue(Radians(value)); }, py::return_value_policy::copy,
      py::arg("radians"));
}
