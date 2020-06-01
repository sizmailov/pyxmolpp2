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
  auto old_begin = m_residue_info.data();
  auto old_end = m_residue_info.data() + m_residue_info.size();
  auto old_insert_pos = mol.m_molecule_info->m_residues_info.m_end;
  if (!old_insert_pos) {
    old_insert_pos = old_end;
  }
  auto new_inserted_it = m_residue_info.insert(m_residue_info.begin() + (old_insert_pos - old_begin),
                                               detail::ResidueInfo{residueName, residueId, {}, mol.m_molecule_info});
  auto new_begin = m_residue_info.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_residue_info.begin());
  auto new_end = m_residue_info.data() + m_residue_info.size();

  // update pointers in mol
  if (!mol.m_molecule_info->m_residues_info.m_begin) {
    mol.m_molecule_info->m_residues_info.m_begin = new_inserted_pos;
    mol.m_molecule_info->m_residues_info.m_end = new_inserted_pos + 1;
  } else {
    mol.m_molecule_info->m_residues_info.m_begin =
        new_begin + (mol.m_molecule_info->m_residues_info.m_begin - old_begin);
    mol.m_molecule_info->m_residues_info.m_end =
        new_begin + (mol.m_molecule_info->m_residues_info.m_end - old_begin) + 1;
  }

  if (new_begin != old_begin) {
    // update pointers in molecules before mol
    for (auto it = m_molecule_info.data(); it != mol.m_molecule_info; ++it) {
      it->m_residues_info.m_begin = new_begin + (it->m_residues_info.m_begin - old_begin);
      it->m_residues_info.m_end = new_begin + (it->m_residues_info.m_end - old_begin);
    }
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_begin, old_insert_pos, new_begin);

    // update pointers in atoms
    for (auto it = new_begin; it != new_inserted_pos; ++it) {
      for (auto ait = it->m_atoms_info.m_begin; ait != it->m_atoms_info.m_end; ++ait) {
        ait->m_residue_info = it;
      }
    }
  }

  // update pointers in molecules after mol
  for (auto it = mol.m_molecule_info + 1; it != m_molecule_info.data() + m_molecule_info.size(); ++it) {
    it->m_residues_info.m_begin = new_begin + (it->m_residues_info.m_begin - old_begin) + 1;
    it->m_residues_info.m_end = new_begin + (it->m_residues_info.m_end - old_begin) + 1;
  }

  selection::Observable<ResidueRef>::notify(&ResidueRef::on_residue_info_move, old_insert_pos, old_end,
                                            new_inserted_pos + 1);
  // update pointers in atoms for shifted residues
  for (auto it = new_inserted_pos + 1; it != new_end; ++it) {
    for (auto ait = it->m_atoms_info.m_begin; ait != it->m_atoms_info.m_end; ++ait) {
      ait->m_residue_info = it;
    }
  }

  ResidueRef result(*this, *new_inserted_pos);
  selection::Observable<ResidueRef>::add_observer(result);
  return result;
}

AtomRef Frame::add_atom(ResidueRef& residue, const AtomName& atomName, const AtomId& atomId) {
  assert(&residue.frame() == this);
  assert(m_residue_info.data() <= residue.m_residue_info);
  assert(residue.m_residue_info < m_residue_info.data() + m_residue_info.size());

  auto old_begin = m_atom_info.data();
  auto old_end = m_atom_info.data() + m_atom_info.size();

  auto old_begin_crd = m_coordinates.data();
  auto old_end_crd = m_coordinates.data() + m_coordinates.size();

  auto old_insert_pos = residue.m_residue_info->m_atoms_info.m_end;
  if (!old_insert_pos) {
    old_insert_pos = old_end;
  }
  auto old_insert_crd_pos = old_begin_crd + (residue.m_residue_info->m_atoms_info.m_end - old_begin);

  auto new_inserted_it = m_atom_info.insert(m_atom_info.begin() + (old_insert_pos - old_begin),
                                            detail::AtomInfo{atomName, atomId, residue.m_residue_info});

  auto new_inserted_crd_it = m_coordinates.insert(m_coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = m_atom_info.data();
  auto new_begin_crd = m_coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_atom_info.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - m_coordinates.begin());

  // update pointers in the residue
  if (!residue.m_residue_info->m_atoms_info.m_begin) {
    residue.m_residue_info->m_atoms_info.m_begin = new_inserted_pos;
    residue.m_residue_info->m_atoms_info.m_end = new_inserted_pos + 1;
  } else {
    residue.m_residue_info->m_atoms_info.m_begin =
        new_begin + (residue.m_residue_info->m_atoms_info.m_begin - old_begin);
    residue.m_residue_info->m_atoms_info.m_end =
        new_begin + (residue.m_residue_info->m_atoms_info.m_end - old_begin) + 1;
  }

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto it = m_residue_info.data(); it != residue.m_residue_info; ++it) {
      it->m_atoms_info.m_begin = new_begin + (it->m_atoms_info.m_begin - old_begin);
      it->m_atoms_info.m_end = new_begin + (it->m_atoms_info.m_end - old_begin);
    }
    selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_begin, old_insert_pos, new_begin);
    selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_begin_crd, old_insert_crd_pos,
                                           new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto it = residue.m_residue_info + 1; it != m_residue_info.data() + m_residue_info.size(); ++it) {
    it->m_atoms_info.m_begin = new_begin + (it->m_atoms_info.m_begin - old_begin) + 1;
    it->m_atoms_info.m_end = new_begin + (it->m_atoms_info.m_end - old_begin) + 1;
  }

  selection::Observable<AtomRef>::notify(&AtomRef::on_atom_info_move, old_insert_pos, old_end, new_inserted_pos + 1);
  selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_insert_crd_pos, old_end_crd,
                                         new_inserted_crd_pos + 1);

  AtomRef result(*this, *new_inserted_pos, *new_inserted_crd_pos);
  selection::Observable<AtomRef>::add_observer(result);
  return result;
}
MoleculeRef Frame::add_molecule(const ChainName& name) {
  auto old_begin = m_molecule_info.data();
  auto old_end = old_begin + m_molecule_info.size();
  m_molecule_info.emplace_back(detail::MoleculeInfo{name});
  auto new_begin = m_molecule_info.data();

  if (old_begin != new_begin) {
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_molecule_info_move, old_begin, old_end, new_begin);
    // update pointers in residues
    for (auto it = new_begin; it != m_molecule_info.data() + m_molecule_info.size(); ++it) {
      for (auto rit = it->m_residues_info.m_begin; rit != it->m_residues_info.m_end; ++rit) {
        rit->m_molecule_info = it;
      }
    }
  }
  MoleculeRef result(*this, m_molecule_info.back());
  selection::Observable<MoleculeRef>::add_observer(result);
  return result;
}
Frame& Frame::operator=(Frame&& other) {
  if (this == &other) {
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
