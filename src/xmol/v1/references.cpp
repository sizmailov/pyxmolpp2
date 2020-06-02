#include "xmol/v1/references.h"
#include "xmol/v1/FrameObserverImpl.h"

using namespace xmol::v1;

AtomRef::AtomRef(BaseAtom& atom)
    : AtomRef(*atom.residue->molecule->frame, atom,
              atom.residue->molecule->frame->coordinates[&atom - atom.residue->molecule->frame->atoms.data()]) {}
void AtomRef::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  if (from_begin <= m_coords && m_coords < from_end) {
    m_coords = to_begin + (m_coords - from_begin);
  }
}
void AtomRef::on_atom_info_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  if (from_begin <= m_atom_info && m_atom_info < from_end) {
    m_atom_info = to_begin + (m_atom_info - from_begin);
  }
}
void AtomRef::name(const AtomName& name) {
  assert(m_frame);
  assert(m_atom_info);
  m_atom_info->name = name;
}
AtomName AtomRef::name() const {
  assert(m_frame);
  assert(m_atom_info);
  return m_atom_info->name;
}

ResidueRef::ResidueRef(BaseResidue& res) : ResidueRef(*res.molecule->frame, res) {}
void ResidueRef::on_residue_info_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_residue_info && m_residue_info < from_end) {
    m_residue_info = to_begin + (m_residue_info - from_begin);
  }
}
AtomRef ResidueRef::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return m_frame->add_atom(*this->m_residue_info, atomName, atomId);
}
ResidueName ResidueRef::name() const {
  assert(m_frame);
  assert(m_residue_info);
  return m_residue_info->name;
}
void ResidueRef::name(const ResidueName& name) {
  assert(m_frame);
  assert(m_residue_info);
  m_residue_info->name = name;
}
bool ResidueRef::empty() const {
  assert(m_frame);
  assert(m_residue_info);
  return m_residue_info->atoms.m_begin == m_residue_info->atoms.m_end;
}
size_t ResidueRef::size() const {
  assert(m_frame);
  assert(m_residue_info);
  return m_residue_info->atoms.m_end - m_residue_info->atoms.m_begin;
}

MoleculeRef::MoleculeRef(BaseMolecule& mol) : MoleculeRef(*mol.frame, mol) {}
ResidueRef MoleculeRef::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return m_frame->add_residue(*this->m_molecule_info, residueName, residueId);
}
void MoleculeRef::on_molecule_info_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin) {
  if (from_begin <= m_molecule_info && m_molecule_info < from_end) {
    m_molecule_info = to_begin + (m_molecule_info - from_begin);
  }
}
MoleculeName MoleculeRef::name() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->name;
}
void MoleculeRef::name(const MoleculeName& name) {
  assert(m_frame);
  assert(m_molecule_info);
  m_molecule_info->name = name;
}
bool MoleculeRef::empty() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->residues.m_begin == m_molecule_info->residues.m_end;
}
size_t MoleculeRef::size() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->residues.m_end - m_molecule_info->residues.m_begin;
}

template class xmol::v1::FrameObserver<xmol::v1::AtomRef>;
template class xmol::v1::FrameObserver<xmol::v1::ResidueRef>;
template class xmol::v1::FrameObserver<xmol::v1::MoleculeRef>;