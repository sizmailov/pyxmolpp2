#include "xmol/v1/Frame.h"

using namespace xmol::v1;

ResidueRef MoleculeRef::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return m_frame->add_residue(*this, residueName, residueId);
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

ResidueRef Frame::add_residue(MoleculeRef& mol, const ResidueName& residueName, const ResidueId& residueId) {
  assert(&mol.frame() == this);
  assert(molecules.data() <= mol.m_molecule_info);
  assert(mol.m_molecule_info < molecules.data() + molecules.size());
  check_references_integrity();
  auto old_begin = residues.data();
  auto old_end = residues.data() + residues.size();
  auto old_insert_pos = mol.m_molecule_info->residues.m_end;

  auto atoms_end = atoms.data();
  if (mol.m_molecule_info->residues.m_end &&
      mol.m_molecule_info->residues.m_end - 1 >= residues.data()) {
    atoms_end = (mol.m_molecule_info->residues.m_end - 1)->atoms.m_end;
  }

  auto new_inserted_it =
      residues.insert(residues.begin() + (old_insert_pos - old_begin),
                            BaseResidue{residueName, residueId, {atoms_end, atoms_end}, mol.m_molecule_info});
  auto new_begin = residues.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - residues.begin());
  auto new_end = residues.data() + residues.size();

  // update pointers in mol
  if (!mol.m_molecule_info->residues.m_begin) {
    mol.m_molecule_info->residues = {new_inserted_pos, new_inserted_pos + 1};
  } else {
    mol.m_molecule_info->residues.rebase(old_begin, new_begin);
    mol.m_molecule_info->residues.m_end++;
  }

  if (new_begin != old_begin) {
    // update pointers in molecules before mol
    for (auto it = molecules.data(); it != mol.m_molecule_info; ++it) {
      it->residues.rebase(old_begin, new_begin);
    }
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_begin, old_insert_pos, new_begin);

    // update pointers in atoms
    for (auto it = new_begin; it != new_inserted_pos; ++it) {
      for (auto& ait : it->atoms) {
        ait.residue = it;
      }
    }
  }

  // update pointers in molecules after mol
  for (auto& it : future::Span(mol.m_molecule_info + 1, molecules.data() + molecules.size())) {
    it.residues.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_insert_pos, old_end,
                                            new_inserted_pos + 1);
  // update pointers in atoms for shifted residues
  for (auto it = new_inserted_pos + 1; it != new_end; ++it) {
    for (auto& ait : it->atoms) {
      ait.residue = it;
    }
  }

  check_references_integrity();
  ResidueRef result(*this, *new_inserted_pos);
  selection::Observable<ResidueRef>::add_observer(result);
  return result;
}

AtomRef Frame::add_atom(ResidueRef& residue, const AtomName& atomName, const AtomId& atomId) {
  assert(&residue.frame() == this);
  assert(residues.data() <= residue.m_residue_info);
  assert(residue.m_residue_info < residues.data() + residues.size());
  check_references_integrity();

  auto old_begin = atoms.data();
  auto old_end = atoms.data() + atoms.size();

  auto old_begin_crd = coordinates.data();
  auto old_end_crd = coordinates.data() + coordinates.size();

  auto old_insert_pos = residue.m_residue_info->atoms.m_end;
  auto old_insert_crd_pos = old_begin_crd + (residue.m_residue_info->atoms.m_end - old_begin);

  auto new_inserted_it = atoms.insert(atoms.begin() + (old_insert_pos - old_begin),
                                            BaseAtom{atomName, atomId, residue.m_residue_info});

  auto new_inserted_crd_it = coordinates.insert(coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = atoms.data();
  auto new_begin_crd = coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - atoms.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - coordinates.begin());

  // update pointers in the residue & increase size
  residue.m_residue_info->atoms.rebase(old_begin, new_begin);
  residue.m_residue_info->atoms.m_end++;

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto& rInfo : future::Span{residues.data(), residue.m_residue_info}) {
      rInfo.atoms.rebase(old_begin, new_begin);
    }
    selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_begin, old_insert_pos, new_begin);
    selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_begin_crd, old_insert_crd_pos,
                                           new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto& it : future::Span{residue.m_residue_info + 1, residues.data() + residues.size()}) {
    it.atoms.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_insert_pos, old_end, new_inserted_pos + 1);
  selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_insert_crd_pos, old_end_crd,
                                         new_inserted_crd_pos + 1);

  check_references_integrity();
  AtomRef result(*this, *new_inserted_pos, *new_inserted_crd_pos);
  selection::Observable<AtomRef>::add_observer(result);
  return result;
}
MoleculeRef Frame::add_molecule(const MoleculeName& name) {
  check_references_integrity();
  auto old_begin = molecules.data();
  auto old_end = old_begin + molecules.size();
  auto residues_end = residues.data() + residues.size();
  molecules.emplace_back(BaseMolecule{this, name, {residues_end, residues_end}});
  auto new_begin = molecules.data();

  if (old_begin != new_begin) {
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_molecule_info_move, old_begin, old_end, new_begin);
    // update pointers in residues
    for (auto& mol_info : future::Span{new_begin, molecules.size()}) {
      for (auto& info : mol_info.residues) {
        info.molecule = &mol_info;
      }
    }
  }
  check_references_integrity();
  MoleculeRef result(*this, molecules.back());
  selection::Observable<MoleculeRef>::add_observer(result);
  return result;
}
Frame& Frame::operator=(Frame&& other) {
  if (this != &other) {
    selection::Observable<AtomRef>::operator=(std::move(other));
    selection::Observable<ResidueRef>::operator=(std::move(other));
    selection::Observable<MoleculeRef>::operator=(std::move(other));
    selection::Observable<AtomRef>::notify(&AtomRef::on_frame_move, other, *this);
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_frame_move, other, *this);
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_frame_move, other, *this);
    atoms = std::move(other.atoms);
    residues = std::move(other.residues);
    molecules = std::move(other.molecules);
    coordinates = std::move(other.coordinates);
  }
  return *this;
}
Frame::Frame(Frame&& other)
    : selection::Observable<AtomRef>(std::move(other)), selection::Observable<ResidueRef>(std::move(other)),
      selection::Observable<MoleculeRef>(std::move(other)), atoms(std::move(other.atoms)),
      residues(std::move(other.residues)), molecules(std::move(other.molecules)),
      coordinates(std::move(other.coordinates)) {
  selection::Observable<AtomRef>::notify(&AtomRef::on_frame_move, other, *this);
  selection::Observable<ResidueRef>::notify(&ResidueRef::on_frame_move, other, *this);
  selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_frame_move, other, *this);
}
Frame::~Frame() {
  selection::Observable<AtomRef>::notify(&AtomRef::on_frame_delete);
  selection::Observable<ResidueRef>::notify(&ResidueRef::on_frame_delete);
  selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_frame_delete);
}
void Frame::check_references_integrity() {
  size_t res_count = 0;
  for (auto& mol_info : molecules) {
    assert(residues.data() <= mol_info.residues.m_begin);
    assert(mol_info.residues.m_begin <= mol_info.residues.m_end);
    assert(mol_info.residues.m_end <= residues.data() + residues.size());
    for (auto& info : mol_info.residues) {
      assert(info.molecule == &mol_info);
    }
    res_count += mol_info.residues.size();
  }
  assert(residues.size() == res_count);
  size_t atom_count = 0;
  for (auto& res_info : residues) {
    assert(atoms.data() <= res_info.atoms.m_begin);
    assert(res_info.atoms.m_begin <= res_info.atoms.m_end);
    assert(res_info.atoms.m_end <= atoms.data() + atoms.size());
    for (auto& info : res_info.atoms) {
      assert(info.residue == &res_info);
    }
    atom_count += res_info.atoms.size();
  }
  assert(atoms.size() == atom_count);
}

void ResidueRef::on_residue_info_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin) {
  if (from_begin <= m_residue_info && m_residue_info < from_end) {
    m_residue_info = to_begin + (m_residue_info - from_begin);
  }
}
AtomRef ResidueRef::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return m_frame->add_atom(*this, atomName, atomId);
}
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
