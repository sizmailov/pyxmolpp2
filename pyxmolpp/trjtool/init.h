#pragma once
#include "../pyxmolpp.h"

namespace pyxmolpp {
namespace trjtool {

void init_exceptions(py::module& trjtool);
void init_DatFile(py::module& trjtool);
}
}
