#pragma once
#include "../pyxmolpp.h"

namespace pyxmolpp {
namespace polymer {

void init_exceptions(py::module& polymer);
void init_Atom(py::module& polymer);
void init_predicates(py::module& polymer);
void init_TorsionAngle(py::module& polymer);

}
}
