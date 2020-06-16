#include "base.h"
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace xmol;

void pyxmolpp::v1::populate(pybind11::class_<xmol::ResidueId>& pyName) {
  pyName.def(py::init<>())
      .def(py::init<residueSerial_t>())
      .def(py::init(
          [](residueSerial_t serial, std::string& name) { return ResidueId(serial, ResidueInsertionCode(name)); }))
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(py::self <= py::self)
      .def(py::self >= py::self)
      .def(py::self < py::self)
      .def(py::self > py::self)
      .def(py::self == residueSerial_t{})
      .def(py::self != residueSerial_t{})
      .def(py::self <= residueSerial_t{})
      .def(py::self >= residueSerial_t{})
      .def(py::self < residueSerial_t{})
      .def(py::self > residueSerial_t{})
      .def(residueSerial_t{} == py::self)
      .def(residueSerial_t{} != py::self)
      .def(residueSerial_t{} <= py::self)
      .def(residueSerial_t{} >= py::self)
      .def(residueSerial_t{} < py::self)
      .def(residueSerial_t{} > py::self)
      .def_readwrite("serial", &ResidueId::serial)
      .def_property(
          "iCode", [](ResidueId& self) { return self.iCode.str(); },
          [](ResidueId& self, const std::string& name) { self.iCode = ResidueInsertionCode(name); })
      .def("__str__", [](ResidueId& rid) { return std::to_string(rid.serial) + rid.iCode.str(); })
      //      .def("__hash__", [](ResidueId& rid) { return std::hash<ResidueId>()(rid); })
      ;
}
