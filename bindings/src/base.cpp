#include "base.h"
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace xmol;

void pyxmolpp::v1::populate(pybind11::class_<xmol::ResidueId>& pyResdiueId) {
  pyResdiueId.def(py::init<>(), "Default constructor")
      .def(py::init<residueSerial_t>(), py::arg("serial"), "Construct from serial")
      .def(py::init(
               [](residueSerial_t serial, std::string& name) { return ResidueId(serial, ResidueInsertionCode(name)); }),
           py::arg("serial"), py::arg("icode"), "Construct from serial and insertion code")
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
      .def_readwrite("serial", &ResidueId::serial, "Serial number")
      .def_property(
          "iCode", [](ResidueId& self) { return self.iCode.str(); },
          [](ResidueId& self, const std::string& name) { self.iCode = ResidueInsertionCode(name); }, "Insertion code")
      .def("__str__", [](ResidueId& rid) { return std::to_string(rid.serial) + rid.iCode.str(); })
      .def("__hash__", [](ResidueId& rid) {
        return std::hash<residueSerial_t>{}(rid.serial)  ^ std::hash<ResidueInsertionCode>{}(rid.iCode);
      });
}
