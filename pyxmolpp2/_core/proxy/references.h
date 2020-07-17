#pragma once
#include <pybind11/pybind11.h>
#include "xmol/Frame.h"

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::proxy::AtomSmartRef>& pyAtom);
void populate(pybind11::class_<xmol::proxy::ResidueSmartRef>& pyResidue);
void populate(pybind11::class_<xmol::proxy::MoleculeSmartRef>& pyMolecule);
void populate(pybind11::class_<xmol::Frame>& pyFrame);

} // namespace pyxmolpp::v1