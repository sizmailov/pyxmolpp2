#include "xmol/v1/references.h"
#include "xmol/v1/FrameObserverImpl.h"

using namespace xmol::v1;

AtomRef::AtomRef(Atom&& atom) : proxy::Atom(atom), FrameObserver<AtomRef>(atom.frame()) {}
void AtomRef::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  if (from_begin <= m_coords && m_coords < from_end) {
    m_coords = to_begin + (m_coords - from_begin);
  }
}
void AtomRef::on_atom_info_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  if (from_begin <= m_atom && m_atom < from_end) {
    m_atom = to_begin + (m_atom - from_begin);
  }
}

void ResidueRef::on_residue_info_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_residue && m_residue < from_end) {
    m_residue = to_begin + (m_residue - from_begin);
  }
}
AtomRef ResidueRef::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return m_frame->add_atom(*this->m_residue, atomName, atomId);
}
ResidueRef::ResidueRef(proxy::Residue&& residue)
        : proxy::Residue(residue), FrameObserver<ResidueRef>(residue.frame()) {}

ResidueRef MoleculeRef::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return m_frame->add_residue(*this->m_molecule, residueName, residueId);
}
void MoleculeRef::on_molecule_info_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin) {
  if (from_begin <= m_molecule && m_molecule < from_end) {
    m_molecule = to_begin + (m_molecule - from_begin);
  }
}
MoleculeRef::MoleculeRef(proxy::Molecule&& molecule)
        : proxy::Molecule(molecule), FrameObserver<MoleculeRef>(molecule.frame()) {}

template class xmol::v1::FrameObserver<xmol::v1::AtomRef>;
template class xmol::v1::FrameObserver<xmol::v1::ResidueRef>;
template class xmol::v1::FrameObserver<xmol::v1::MoleculeRef>;