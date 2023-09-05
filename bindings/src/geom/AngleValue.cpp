#include "AngleValue.h"

#include <pybind11/operators.h>

using namespace xmol::geom;
namespace py = pybind11;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::AngleValue>& pyAngleValue) {

  pyAngleValue.def(py::self + py::self)
      .def(py::self - py::self)
      .def(-py::self)
      .def(py::self / double())
      .def(py::self * double())
      .def(double() * py::self)
      .def(py::self <= py::self)
      .def(py::self >= py::self)
      .def(py::self < py::self)
      .def(py::self > py::self)
      .def_property_readonly("degrees", &AngleValue::degrees, py::return_value_policy::copy, "Angle value in degrees")
      .def_property_readonly("radians", &AngleValue::radians, py::return_value_policy::copy, "Angle value in radians")
      .def("to_standard_range", &AngleValue::to_standard_range, py::return_value_policy::copy,
           "Wraps value to :math:`[-\\pi..\\pi)` range")

      .def("sin", [](AngleValue& value) { return sin(value); })
      .def("cos", [](AngleValue& value) { return cos(value); })
      .def("tan", [](AngleValue& value) { return tan(value); })
      .def("abs", [](AngleValue& value) { return fabs(value); });
}

void pyxmolpp::v1::define_angle_free_functions(pybind11::module& module) {
  module.def("degrees_to_radians", degrees_to_radians, py::arg("degrees"));
  module.def("radians_to_degrees", radians_to_degrees, py::arg("radians"));
  module.def(
      "Degrees", [](double value) { return AngleValue(Degrees(value)); }, py::arg("degrees"));
  module.def(
      "Radians", [](double value) { return AngleValue(Radians(value)); }, py::arg("radians"));
}
