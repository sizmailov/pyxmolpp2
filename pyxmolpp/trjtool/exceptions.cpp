#include "init.h"

#include "xmol/trjtool/exceptions.h"

void pyxmolpp::trjtool::init_exceptions(pybind11::module& trjtool) {
  auto base = py::register_exception<xmol::trjtool::TrjtoolException>(trjtool, "TrjtoolException");
  py::register_exception<xmol::trjtool::corrupted_file>(trjtool, "corrupted_file", base.ptr());
  py::register_exception<xmol::trjtool::unexpected_eof>(trjtool, "unexpected_eof", base.ptr());

}
