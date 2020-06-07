#include "xmol/v1/proxy/smart/references.h"
#include "xmol/v1/proxy/smart/FrameObserverImpl.h"

using namespace xmol::v1::proxy::smart;

AtomSmartRef::AtomSmartRef(AtomRef atom) : FrameObserver<AtomSmartRef>(&atom.frame()), m_ref(atom) {
  frame().reg(*this);
}
void AtomSmartRef::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  if (from_begin <= m_ref.m_coords && m_ref.m_coords < from_end) {
    m_ref.m_coords = to_begin + (m_ref.m_coords - from_begin);
  }
}
void AtomSmartRef::on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  if (from_begin <= m_ref.m_atom && m_ref.m_atom < from_end) {
    m_ref.m_atom = to_begin + (m_ref.m_atom - from_begin);
  }
}

void ResidueSmartRef::on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_ref.m_residue && m_ref.m_residue < from_end) {
    m_ref.m_residue = to_begin + (m_ref.m_residue - from_begin);
  }
}

ResidueSmartRef::ResidueSmartRef(ResidueRef residue)
    : FrameObserver<ResidueSmartRef>(&residue.frame()), m_ref(residue) {
  frame().reg(*this);
}

void MoleculeSmartRef::on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end,
                                              BaseMolecule* to_begin) {
  if (from_begin <= m_ref.m_molecule && m_ref.m_molecule < from_end) {
    m_ref.m_molecule = to_begin + (m_ref.m_molecule - from_begin);
  }
}

MoleculeSmartRef::MoleculeSmartRef(proxy::MoleculeRef molecule)
    : FrameObserver<MoleculeSmartRef>(&molecule.frame()), m_ref(molecule) {
  frame().reg(*this);
}

template class xmol::v1::proxy::smart::FrameObserver<xmol::v1::proxy::smart::AtomSmartRef>;
template class xmol::v1::proxy::smart::FrameObserver<xmol::v1::proxy::smart::ResidueSmartRef>;
template class xmol::v1::proxy::smart::FrameObserver<xmol::v1::proxy::smart::MoleculeSmartRef>;