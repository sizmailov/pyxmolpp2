#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "pybind11/operators.h"

void pyxmolpp::polymer::init_ChainName(detail::FWD& fwd, py::module& polymer) {

  using namespace xmol::polymer;

  (*fwd.pyChainName)
      .def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &ChainName::str)
      .def("__hash__", &ChainName::value)
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__str__", [](ChainName& name) { return  name.str() ; })
      .def("__repr__", [](ChainName& name) {
        return "<pyxmolpp2.polymer.ChainName \"" + name.str() + "\" at 0x" +
            xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });
}
