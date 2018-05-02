#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_trjtool(pybind11::module& trjtool) {
  pyxmolpp::trjtool::init_exceptions(trjtool);
  pyxmolpp::trjtool::init_DatFile(trjtool);
}
