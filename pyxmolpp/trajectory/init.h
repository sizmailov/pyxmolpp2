#pragma once

#include "../pyxmolpp.h"

namespace pyxmolpp {
namespace trajectory {

void init_exceptions(py::module& trajectory);
void init_Trajectory(py::module& trajectory);

}
}
