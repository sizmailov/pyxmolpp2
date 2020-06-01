#include "xmol/v1/Frame.h"

using namespace xmol::v1;

ResidueRef MoleculeRef::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return m_frame->add_residue(*this, residueName, residueId);
}
void MoleculeRef::on_molecule_info_move(detail::MoleculeInfo* from_begin, detail::MoleculeInfo* from_end,
                                        detail::MoleculeInfo* to_begin) {
  if (from_begin <= m_molecule_info && m_molecule_info < from_end) {
    m_molecule_info = to_begin + (m_molecule_info - from_begin);
  }
}
ChainName MoleculeRef::name() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->m_name;
}
void MoleculeRef::name(const ChainName& name) {
  assert(m_frame);
  assert(m_molecule_info);
  m_molecule_info->m_name = name;
}
bool MoleculeRef::empty() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->m_residues_info.m_begin == m_molecule_info->m_residues_info.m_end;
}
size_t MoleculeRef::size() const {
  assert(m_frame);
  assert(m_molecule_info);
  return m_molecule_info->m_residues_info.m_end - m_molecule_info->m_residues_info.m_begin;
}

ResidueRef Frame::add_residue(MoleculeRef& mol, const ResidueName& residueName, const ResidueId& residueId) {
  assert(&mol.frame() == this);
  assert(m_molecule_info.data() <= mol.m_molecule_info);
  assert(mol.m_molecule_info < m_molecule_info.data() + m_molecule_info.size());
  check_references_integrity();
  auto old_begin = m_residue_info.data();
  auto old_end = m_residue_info.data() + m_residue_info.size();
  auto old_insert_pos = mol.m_molecule_info->m_residues_info.m_end;

  auto atoms_end = m_atom_info.data();
  if (mol.m_molecule_info->m_residues_info.m_end &&
      mol.m_molecule_info->m_residues_info.m_end - 1 >= m_residue_info.data()) {
    atoms_end = (mol.m_molecule_info->m_residues_info.m_end - 1)->m_atoms_info.m_end;
  }

  auto new_inserted_it =
      m_residue_info.insert(m_residue_info.begin() + (old_insert_pos - old_begin),
                            detail::ResidueInfo{residueName, residueId, {atoms_end, atoms_end}, mol.m_molecule_info});
  auto new_begin = m_residue_info.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_residue_info.begin());
  auto new_end = m_residue_info.data() + m_residue_info.size();

  // update pointers in mol
  if (!mol.m_molecule_info->m_residues_info.m_begin) {
    mol.m_molecule_info->m_residues_info = {new_inserted_pos, new_inserted_pos + 1};
  } else {
    mol.m_molecule_info->m_residues_info.rebase(old_begin, new_begin);
    mol.m_molecule_info->m_residues_info.m_end++;
  }

  if (new_begin != old_begin) {
    // update pointers in molecules before mol
    for (auto it = m_molecule_info.data(); it != mol.m_molecule_info; ++it) {
      it->m_residues_info.rebase(old_begin, new_begin);
    }
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_begin, old_insert_pos, new_begin);

    // update pointers in atoms
    for (auto it = new_begin; it != new_inserted_pos; ++it) {
      for (auto& ait : it->m_atoms_info) {
        ait.m_residue_info = it;
      }
    }
  }

  // update pointers in molecules after mol
  for (auto& it : detail::Span(mol.m_molecule_info + 1, m_molecule_info.data() + m_molecule_info.size())) {
    it.m_residues_info.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_insert_pos, old_end,
                                            new_inserted_pos + 1);
  // update pointers in atoms for shifted residues
  for (auto it = new_inserted_pos + 1; it != new_end; ++it) {
    for (auto& ait : it->m_atoms_info) {
      ait.m_residue_info = it;
    }
  }

  check_references_integrity();
  ResidueRef result(*this, *new_inserted_pos);
  selection::Observable<ResidueRef>::add_observer(result);
  return result;
}

AtomRef Frame::add_atom(ResidueRef& residue, const AtomName& atomName, const AtomId& atomId) {
  assert(&residue.frame() == this);
  assert(m_residue_info.data() <= residue.m_residue_info);
  assert(residue.m_residue_info < m_residue_info.data() + m_residue_info.size());
  check_references_integrity();

  auto old_begin = m_atom_info.data();
  auto old_end = m_atom_info.data() + m_atom_info.size();

  auto old_begin_crd = m_coordinates.data();
  auto old_end_crd = m_coordinates.data() + m_coordinates.size();

  auto old_insert_pos = residue.m_residue_info->m_atoms_info.m_end;
  auto old_insert_crd_pos = old_begin_crd + (residue.m_residue_info->m_atoms_info.m_end - old_begin);

  auto new_inserted_it = m_atom_info.insert(m_atom_info.begin() + (old_insert_pos - old_begin),
                                            detail::AtomInfo{atomName, atomId, residue.m_residue_info});

  auto new_inserted_crd_it = m_coordinates.insert(m_coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = m_atom_info.data();
  auto new_begin_crd = m_coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_atom_info.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - m_coordinates.begin());

  // update pointers in the residue & increase size
  residue.m_residue_info->m_atoms_info.rebase(old_begin, new_begin);
  residue.m_residue_info->m_atoms_info.m_end++;

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto& rInfo : detail::Span{m_residue_info.data(), residue.m_residue_info}) {
      rInfo.m_atoms_info.rebase(old_begin, new_begin);
    }
    selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_begin, old_insert_pos, new_begin);
    selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_begin_crd, old_insert_crd_pos,
                                           new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto& it : detail::Span{residue.m_residue_info + 1, m_residue_info.data() + m_residue_info.size()}) {
    it.m_atoms_info.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_insert_pos, old_end, new_inserted_pos + 1);
  selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_insert_crd_pos, old_end_crd,
                                         new_inserted_crd_pos + 1);

  check_references_integrity();
  AtomRef result(*this, *new_inserted_pos, *new_inserted_crd_pos);
  selection::Observable<AtomRef>::add_observer(result);
  return result;
}
MoleculeRef Frame::add_molecule(const ChainName& name) {
  check_references_integrity();
  auto old_begin = m_molecule_info.data();
  auto old_end = old_begin + m_molecule_info.size();
  auto residues_end = m_residue_info.data() + m_residue_info.size();
  m_molecule_info.emplace_back(detail::MoleculeInfo{name, {residues_end, residues_end}});
  auto new_begin = m_molecule_info.data();

  if (old_begin != new_begin) {
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_molecule_info_move, old_begin, old_end, new_begin);
    // update pointers in residues
    for (auto& mol_info : detail::Span{new_begin, m_molecule_info.size()}) {
      for (auto& info : mol_info.m_residues_info) {
        info.m_molecule_info = &mol_info;
      }
    }
  }
  check_references_integrity();
  MoleculeRef result(*this, m_molecule_info.back());
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
    m_atom_info = std::move(other.m_atom_info);
    m_residue_info = std::move(other.m_residue_info);
    m_molecule_info = std::move(other.m_molecule_info);
    m_coordinates = std::move(other.m_coordinates);
  }
  return *this;
}
Frame::Frame(Frame&& other)
    : selection::Observable<AtomRef>(std::move(other)), selection::Observable<ResidueRef>(std::move(other)),
      selection::Observable<MoleculeRef>(std::move(other)), m_atom_info(std::move(other.m_atom_info)),
      m_residue_info(std::move(other.m_residue_info)), m_molecule_info(std::move(other.m_molecule_info)),
      m_coordinates(std::move(other.m_coordinates)) {
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
  for (auto& mol_info : m_molecule_info) {
    assert(m_residue_info.data() <= mol_info.m_residues_info.m_begin);
    assert(mol_info.m_residues_info.m_begin <= mol_info.m_residues_info.m_end);
    assert(mol_info.m_residues_info.m_end <= m_residue_info.data() + m_residue_info.size());
    for (auto& info : mol_info.m_residues_info) {
      assert(info.m_molecule_info == &mol_info);
    }
    res_count += mol_info.m_residues_info.size();
  }
  assert(m_residue_info.size() == res_count);
  size_t atom_count = 0;
  for (auto& res_info : m_residue_info) {
    assert(m_atom_info.data() <= res_info.m_atoms_info.m_begin);
    assert(res_info.m_atoms_info.m_begin <= res_info.m_atoms_info.m_end);
    assert(res_info.m_atoms_info.m_end <= m_atom_info.data() + m_atom_info.size());
    for (auto& info : res_info.m_atoms_info) {
      assert(info.m_residue_info == &res_info);
    }
    atom_count += res_info.m_atoms_info.size();
  }
  assert(m_atom_info.size() == atom_count);
}

void ResidueRef::on_residue_info_move(detail::ResidueInfo* from_begin, detail::ResidueInfo* from_end,
                                      detail::ResidueInfo* to_begin) {
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
void AtomRef::on_atom_info_move(detail::AtomInfo* from_begin, detail::AtomInfo* from_end, detail::AtomInfo* to_begin) {
  if (from_begin <= m_atom_info && m_atom_info < from_end) {
    m_atom_info = to_begin + (m_atom_info - from_begin);
  }
}
