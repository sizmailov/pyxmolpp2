#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "pybind11/operators.h"

void pyxmolpp::polymer::init_ResidueName(detail::FWD& fwd, py::module& polymer) {

  using namespace xmol::polymer;

  (*fwd.pyResidueName)
      .def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &ResidueName::str)
      .def("__hash__", &ResidueName::value)
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__str__", [](ResidueName& name) { return name.str(); })
      .def("__repr__", [](ResidueName& name) {
        return "<pyxmolpp2.polymer.ResidueName \"" + name.str() + "\" at 0x" +
            xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });
}
