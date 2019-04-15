#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "pybind11/operators.h"

void pyxmolpp::polymer::init_ResidueId(detail::FWD& fwd, py::module& polymer) {

  using namespace xmol::polymer;

  (*fwd.pyResidueId)
      .def(py::init<>())
      .def(py::init<residueSerial_t>())
      .def(py::init<residueSerial_t, const ResidueInsertionCode&>())
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def(py::self<=py::self)
      .def(py::self>=py::self)
      .def(py::self<py::self)
      .def(py::self>py::self)
      .def(py::self==residueSerial_t{})
      .def(py::self!=residueSerial_t{})
      .def(py::self<=residueSerial_t{})
      .def(py::self>=residueSerial_t{})
      .def(py::self<residueSerial_t{})
      .def(py::self>residueSerial_t{})
      .def(residueSerial_t{}==py::self)
      .def(residueSerial_t{}!=py::self)
      .def(residueSerial_t{}<=py::self)
      .def(residueSerial_t{}>=py::self)
      .def(residueSerial_t{}<py::self)
      .def(residueSerial_t{}>py::self)
      .def_readwrite("serial", &ResidueId::serial)
      .def_readwrite("iCode", &ResidueId::iCode)
      .def("__str__", [](ResidueId& rid) { return std::to_string(rid.serial) + rid.iCode.str(); })
      .def("__hash__", [](ResidueId& rid) { return std::hash<ResidueId>()(rid); })
      .def("__repr__", [](ResidueId& rid) {
        return "<pyxmolpp2.polymer.ResidueId\"" + std::to_string(rid.serial) + rid.iCode.str() + "\" at 0x" +
            xmol::utils::string::int2hex((uint64_t)(std::addressof(rid))) + ">";
      });
}
