#pragma once
#include <pybind11/pybind11.h>
#include "xmol/Frame.h"

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::proxy::smart::AtomSmartRef>& pyAtom);
void populate(pybind11::class_<xmol::proxy::smart::ResidueSmartRef>& pyResidue);
void populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartRef>& pyMolecule);
void populate(pybind11::class_<xmol::Frame>& pyFrame);

} // namespace pyxmolpp::v1