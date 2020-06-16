#pragma once
#include "xmol/geom/UnitCell.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::geom::UnitCell>& pyXYZ);

}