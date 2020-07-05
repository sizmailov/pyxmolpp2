#pragma once

#include "xmol/io/GromacsXtcFile.h"
#include "xmol/io/xdr/XtcWriter.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::io::GromacsXtcFile, xmol::trajectory::TrajectoryInputFile>& pyGromacsXtc);
void populate(pybind11::class_<xmol::io::xdr::XtcWriter>& pyXtcWriter);

}