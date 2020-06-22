#pragma once
#include "xmol/Frame.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void init_predicates(pybind11::module& m);

} // namespace pyxmolpp::v1