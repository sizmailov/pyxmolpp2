#pragma once

#include "xmol/io/TrjtoolDatFile.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::io::TrjtoolDatFile, xmol::trajectory::TrajectoryInputFile>& pyDatFile);

}