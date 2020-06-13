#pragma once
#include <pybind11/pybind11.h>
#include "xmol/proxy/smart/selections.h"

namespace pyxmolpp::v1{

void populate(pybind11::class_<xmol::proxy::smart::CoordSmartSelection>& pyCoordSelection);
void populate(pybind11::class_<xmol::proxy::smart::AtomSmartSelection>& sel);
void populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSelection>& pyResidueSelection);
void populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSelection>& pyMoleculeSelection);

}