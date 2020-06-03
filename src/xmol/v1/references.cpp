#include "xmol/v1/references.h"
#include "xmol/v1/FrameObserverImpl.h"

using namespace xmol::v1;

AtomRef::AtomRef(Atom&& atom) : proxy::Atom(atom), FrameObserver<AtomRef>(atom.frame()) {
    frame().reg(*this);
}
void AtomRef::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  if (from_begin <= m_coords && m_coords < from_end) {
    m_coords = to_begin + (m_coords - from_begin);
  }
}
void AtomRef::on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  if (from_begin <= m_atom && m_atom < from_end) {
    m_atom = to_begin + (m_atom - from_begin);
  }
}

void ResidueRef::on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_residue && m_residue < from_end) {
    m_residue = to_begin + (m_residue - from_begin);
  }
}

ResidueRef::ResidueRef(proxy::Residue&& residue)
        : proxy::Residue(residue), FrameObserver<ResidueRef>(residue.frame()) {
    frame().reg(*this);
}


void MoleculeRef::on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin) {
  if (from_begin <= m_molecule && m_molecule < from_end) {
    m_molecule = to_begin + (m_molecule - from_begin);
  }
}
MoleculeRef::MoleculeRef(proxy::Molecule&& molecule)
        : proxy::Molecule(molecule), FrameObserver<MoleculeRef>(molecule.frame()) {
  frame().reg(*this);
}

template class xmol::v1::FrameObserver<xmol::v1::AtomRef>;
template class xmol::v1::FrameObserver<xmol::v1::ResidueRef>;
template class xmol::v1::FrameObserver<xmol::v1::MoleculeRef>;