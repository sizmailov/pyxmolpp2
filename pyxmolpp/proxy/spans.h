#pragma once
#include "xmol/proxy/smart/spans.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::proxy::smart::CoordSmartSpan>& span);
void populate(pybind11::class_<xmol::proxy::smart::AtomSmartSpan>& span);
void populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSpan>& pyResidue);
void populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSpan>& pyMolecule);

} // namespace pyxmolpp::v1
