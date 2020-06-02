
#include <xmol/v1/references.h>

#include "xmol/v1/Frame.h"

using namespace xmol::v1;

MoleculeRef Frame::add_molecule(const MoleculeName& name) {
  MoleculeRef result(add_molecule(name, base_tag{}));
  selection::Observable<MoleculeRef>::add_observer(result);
  return result;
}

BaseResidue& Frame::add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId,
                                base_tag) {
  assert(mol.frame == this);
  assert(molecules.data() <= &mol);
  assert(&mol < molecules.data() + molecules.size());
  check_references_integrity();
  auto old_begin = residues.data();
  auto old_end = residues.data() + residues.size();
  auto old_insert_pos = mol.residues.m_end;

  auto atoms_end = atoms.data();
  if (mol.residues.m_end && mol.residues.m_end - 1 >= residues.data()) {
    atoms_end = (mol.residues.m_end - 1)->atoms.m_end;
  }

  auto new_inserted_it = residues.insert(residues.begin() + (old_insert_pos - old_begin),
                                         BaseResidue{residueName, residueId, {atoms_end, atoms_end}, &mol});
  auto new_begin = residues.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - residues.begin());
  auto new_end = residues.data() + residues.size();

  // update pointers in mol
  if (!mol.residues.m_begin) {
    mol.residues = {new_inserted_pos, new_inserted_pos + 1};
  } else {
    mol.residues.rebase(old_begin, new_begin);
    mol.residues.m_end++;
  }

  if (new_begin != old_begin) {
    // update pointers in molecules before mol
    for (auto it = molecules.data(); it != &mol; ++it) {
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
  for (auto& it : future::Span(&mol + 1, molecules.data() + molecules.size())) {
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
  return *new_inserted_pos;
}

BaseAtom& Frame::add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId, base_tag) {
  assert(residue.molecule);
  assert(residue.molecule->frame == this);
  assert(residues.data() <= &residue);
  assert(&residue < residues.data() + residues.size());
  check_references_integrity();

  auto old_begin = atoms.data();
  auto old_end = atoms.data() + atoms.size();

  auto old_begin_crd = coordinates.data();
  auto old_end_crd = coordinates.data() + coordinates.size();

  auto old_insert_pos = residue.atoms.m_end;
  auto old_insert_crd_pos = old_begin_crd + (residue.atoms.m_end - old_begin);

  auto new_inserted_it =
      atoms.insert(atoms.begin() + (old_insert_pos - old_begin), BaseAtom{atomName, atomId, &residue});

  auto new_inserted_crd_it = coordinates.insert(coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = atoms.data();
  auto new_begin_crd = coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - atoms.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - coordinates.begin());

  // update pointers in the residue & increase size
  residue.atoms.rebase(old_begin, new_begin);
  residue.atoms.m_end++;

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto& rInfo : future::Span{residues.data(), &residue}) {
      rInfo.atoms.rebase(old_begin, new_begin);
    }
    selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_begin, old_insert_pos, new_begin);
    selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_begin_crd, old_insert_crd_pos,
                                           new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto& it : future::Span{&residue + 1, residues.data() + residues.size()}) {
    it.atoms.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_insert_pos, old_end, new_inserted_pos + 1);
  selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_insert_crd_pos, old_end_crd,
                                         new_inserted_crd_pos + 1);

  check_references_integrity();
  return *new_inserted_pos;
}
ResidueRef Frame::add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId) {
  ResidueRef result(add_residue(mol, residueName, residueId, base_tag{}));
  selection::Observable<ResidueRef>::add_observer(result);
  return result;
}
AtomRef Frame::add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId) {
  AtomRef result(add_atom(residue, atomName, atomId, base_tag{}));
  selection::Observable<AtomRef>::add_observer(result);
  return result;
}
BaseMolecule& Frame::add_molecule(const MoleculeName& name, Frame::base_tag) {
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
  return molecules.back();
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
#ifdef NDEBUG
  return; // disable completely in release mode
#endif
  // disable except for smallest debug cases
  if (molecules.size() > 10 || residues.size() > 10 || atoms.size() > 10) {
    return;
  }
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
void Frame::reserve_molecules(size_t n) { molecules.reserve(n); }
void Frame::reserve_atoms(size_t n) {
  atoms.reserve(n);
  coordinates.reserve(n);
}
void Frame::reserve_residues(size_t n) { residues.reserve(n); }
