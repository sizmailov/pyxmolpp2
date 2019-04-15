#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "pybind11/operators.h"


void pyxmolpp::polymer::init_AtomName(detail::FWD& fwd, py::module& polymer) {
  using namespace xmol::polymer;

  (*fwd.pyAtomName)
      .def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &AtomName::str)
      .def("__hash__", &AtomName::value)
      .def("__str__", [](AtomName& name) { return name.str(); })
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__repr__", [](AtomName& name) {
        return "<pyxmolpp2.polymer.AtomName \"" + name.str() + "\" at 0x" +
            xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });
}
