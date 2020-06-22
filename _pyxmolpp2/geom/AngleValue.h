#pragma once
#include "xmol/geom/AngleValue.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::geom::AngleValue>& pyAngleValue);
void define_angle_free_functions(pybind11::module& module);

}