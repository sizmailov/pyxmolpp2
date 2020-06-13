#pragma once
#include <pybind11/pybind11.h>
#include "xmol/proxy/smart/spans.h"

namespace pyxmolpp::v1{

void populate(pybind11::class_<xmol::proxy::smart::CoordSmartSpan>& span);
void populate(pybind11::class_<xmol::proxy::smart::AtomSmartSpan>& pyAtom);
void populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSpan>& pyResidue);
void populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSpan>& pyMolecule);

}