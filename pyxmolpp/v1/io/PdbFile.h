#pragma once

#include "xmol/io/PdbInputFile.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::io::PdbInputFile, xmol::trajectory::TrajectoryInputFile>& pyPdbInputFile);

}