#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "pybind11/operators.h"

void pyxmolpp::polymer::init_ResidueInsertionCode(detail::FWD& fwd, py::module& polymer) {

  using namespace xmol::polymer;

  (*fwd.pyResidueInsertionCode)
      .def(py::init<const std::string&>())
      .def_property_readonly("str", &ResidueInsertionCode::str)
      .def("__hash__", &ResidueInsertionCode::value)
      .def("__str__", [](ResidueInsertionCode& name) { return name.str(); })
      .def("__repr__", [](ResidueInsertionCode& name) {
        return "<pyxmolpp2.polymer.ResidueInsertionCode\"" + name.str() + "\" at 0x" +
            xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });
}
