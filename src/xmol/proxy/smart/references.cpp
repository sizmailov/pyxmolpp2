#include "xmol/proxy/references/AtomSmartRef.h"
#include "xmol/proxy/references/ResidueSmartRef.h"
#include "xmol/proxy/references/MoleculeSmartRef.h"
#include "xmol/proxy/FrameObserverImpl.h"

using namespace xmol::proxy;

ConstAtomSmartRef::ConstAtomSmartRef(AtomRef atom) : FrameObserver<ConstAtomSmartRef>(&atom.frame()), m_ref(atom) {
  atom.frame().reg(*this);
}
void ConstAtomSmartRef::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  if (from_begin <= m_ref.coord_ptr() && m_ref.coord_ptr() < from_end) {
    m_ref.rebase(from_begin,to_begin);
  }
}
void ConstAtomSmartRef::on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  if (from_begin <= m_ref.atom_ptr() && m_ref.atom_ptr() < from_end) {
    m_ref.rebase(from_begin, to_begin);
  }
}

void ResidueSmartRef::on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_ref.res_ptr() && m_ref.res_ptr() < from_end) {
    m_ref.rebase(from_begin, to_begin);
  }
}

ResidueSmartRef::ResidueSmartRef(ResidueRef residue)
    : FrameObserver<ResidueSmartRef>(&residue.frame()), m_ref(residue) {
  frame().reg(*this);
}

void MoleculeSmartRef::on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end,
                                              BaseMolecule* to_begin) {
  if (from_begin <= m_ref.mol_ptr() && m_ref.mol_ptr() < from_end) {
    m_ref.rebase(from_begin, to_begin);
  }
}

MoleculeSmartRef::MoleculeSmartRef(proxy::MoleculeRef molecule)
    : FrameObserver<MoleculeSmartRef>(&molecule.frame()), m_ref(molecule) {
  frame().reg(*this);
}

template class xmol::proxy::FrameObserver<xmol::proxy::ConstAtomSmartRef>;
template class xmol::proxy::FrameObserver<xmol::proxy::ResidueSmartRef>;
template class xmol::proxy::FrameObserver<xmol::proxy::MoleculeSmartRef>;