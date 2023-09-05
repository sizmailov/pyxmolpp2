#pragma once

#include "xmol/base.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::ResidueId>& pyName);

}