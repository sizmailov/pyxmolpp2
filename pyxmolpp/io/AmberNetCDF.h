#pragma once

#include "xmol/io/AmberNetCDF.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::io::AmberNetCDF, xmol::trajectory::TrajectoryInputFile>& pyAmberNetCDF);

}