
#include <xmol/v1/references.h>

#include "xmol/v1/Frame.h"

using namespace xmol::v1;

BaseResidue& Frame::add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId) {
  assert(mol.frame == this);
  assert(m_molecules.data() <= &mol);
  assert(&mol < m_molecules.data() + m_molecules.size());
  check_references_integrity();
  auto old_begin = m_residues.data();
  auto old_end = m_residues.data() + m_residues.size();
  auto old_insert_pos = mol.residues.m_end;

  auto atoms_end = m_atoms.data();
  if (mol.residues.m_end && mol.residues.m_end - 1 >= m_residues.data()) {
    atoms_end = (mol.residues.m_end - 1)->atoms.m_end;
  }

  auto new_inserted_it = m_residues.insert(m_residues.begin() + (old_insert_pos - old_begin),
                                           BaseResidue{residueName, residueId, {atoms_end, atoms_end}, &mol});
  auto new_begin = m_residues.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_residues.begin());
  auto new_end = m_residues.data() + m_residues.size();

  // update pointers in mol
  if (!mol.residues.m_begin) {
    mol.residues = {new_inserted_pos, new_inserted_pos + 1};
  } else {
    mol.residues.rebase(old_begin, new_begin);
    mol.residues.m_end++;
  }

  if (new_begin != old_begin) {
    // update pointers in molecules before mol
    for (auto it = m_molecules.data(); it != &mol; ++it) {
      it->residues.rebase(old_begin, new_begin);
    }
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_base_residues_move, old_begin, old_insert_pos, new_begin);

    // update pointers in atoms
    for (auto it = new_begin; it != new_inserted_pos; ++it) {
      for (auto& ait : it->atoms) {
        ait.residue = it;
      }
    }
  }

  // update pointers in molecules after mol
  for (auto& it : future::Span(&mol + 1, m_molecules.data() + m_molecules.size())) {
    it.residues.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<ResidueRef>::notify(&ResidueRef::on_base_residues_move, old_insert_pos, old_end,
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

BaseAtom& Frame::add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId) {
  assert(residue.molecule);
  assert(residue.molecule->frame == this);
  assert(m_residues.data() <= &residue);
  assert(&residue < m_residues.data() + m_residues.size());
  check_references_integrity();

  auto old_begin = m_atoms.data();
  auto old_end = m_atoms.data() + m_atoms.size();

  auto old_begin_crd = coordinates.data();
  auto old_end_crd = coordinates.data() + coordinates.size();

  auto old_insert_pos = residue.atoms.m_end;
  auto old_insert_crd_pos = old_begin_crd + (residue.atoms.m_end - old_begin);

  auto new_inserted_it =
      m_atoms.insert(m_atoms.begin() + (old_insert_pos - old_begin), BaseAtom{atomName, atomId, &residue});

  auto new_inserted_crd_it = coordinates.insert(coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = m_atoms.data();
  auto new_begin_crd = coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_atoms.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - coordinates.begin());

  // update pointers in the residue & increase size
  residue.atoms.rebase(old_begin, new_begin);
  residue.atoms.m_end++;

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto& rInfo : future::Span{m_residues.data(), &residue}) {
      rInfo.atoms.rebase(old_begin, new_begin);
    }
    selection::Observable<AtomRef>::notify(&AtomRef::on_base_atoms_move, old_begin, old_insert_pos, new_begin);
    selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_begin_crd, old_insert_crd_pos,
                                           new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto& it : future::Span{&residue + 1, m_residues.data() + m_residues.size()}) {
    it.atoms.rebase(old_begin, new_begin + 1);
  }

  selection::Observable<AtomRef>::notify(&AtomRef::on_base_atoms_move, old_insert_pos, old_end, new_inserted_pos + 1);
  selection::Observable<AtomRef>::notify(&AtomRef::on_coordinates_move, old_insert_crd_pos, old_end_crd,
                                         new_inserted_crd_pos + 1);

  check_references_integrity();
  return *new_inserted_pos;
}

proxy::Molecule Frame::add_molecule(const MoleculeName& name) {
  check_references_integrity();
  auto old_begin = m_molecules.data();
  auto old_end = old_begin + m_molecules.size();
  auto residues_end = m_residues.data() + m_residues.size();
  m_molecules.emplace_back(BaseMolecule{this, name, {residues_end, residues_end}});
  auto new_begin = m_molecules.data();

  if (old_begin != new_begin) {
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_base_molecules_move, old_begin, old_end, new_begin);
    // update pointers in residues
    for (auto& mol_info : future::Span{new_begin, m_molecules.size()}) {
      for (auto& info : mol_info.residues) {
        info.molecule = &mol_info;
      }
    }
  }
  check_references_integrity();
  return proxy::Molecule(m_molecules.back());
}
Frame& Frame::operator=(Frame&& other) {
  if (this != &other) {
    selection::Observable<AtomRef>::operator=(std::move(other));
    selection::Observable<ResidueRef>::operator=(std::move(other));
    selection::Observable<MoleculeRef>::operator=(std::move(other));
    selection::Observable<AtomRef>::notify(&AtomRef::on_frame_move, other, *this);
    selection::Observable<ResidueRef>::notify(&ResidueRef::on_frame_move, other, *this);
    selection::Observable<MoleculeRef>::notify(&MoleculeRef::on_frame_move, other, *this);
    m_atoms = std::move(other.m_atoms);
    m_residues = std::move(other.m_residues);
    m_molecules = std::move(other.m_molecules);
    coordinates = std::move(other.coordinates);
  }
  return *this;
}
Frame::Frame(Frame&& other)
    : selection::Observable<AtomRef>(std::move(other)), selection::Observable<ResidueRef>(std::move(other)),
      selection::Observable<MoleculeRef>(std::move(other)), m_atoms(std::move(other.m_atoms)),
      m_residues(std::move(other.m_residues)), m_molecules(std::move(other.m_molecules)),
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
  return; // disables check completely in release mode
#endif
  // disable except for smallest debug cases
  if (m_molecules.size() > 10 || m_residues.size() > 10 || m_atoms.size() > 10) {
    return;
  }
  size_t res_count = 0;
  for (auto& mol_info : m_molecules) {
    assert(m_residues.data() <= mol_info.residues.m_begin);
    assert(mol_info.residues.m_begin <= mol_info.residues.m_end);
    assert(mol_info.residues.m_end <= m_residues.data() + m_residues.size());
    for (auto& info : mol_info.residues) {
      assert(info.molecule == &mol_info);
      static_cast<void>(info);
    }
    res_count += mol_info.residues.size();
  }
  assert(m_residues.size() == res_count);
  size_t atom_count = 0;
  for (auto& res_info : m_residues) {
    assert(m_atoms.data() <= res_info.atoms.m_begin);
    assert(res_info.atoms.m_begin <= res_info.atoms.m_end);
    assert(res_info.atoms.m_end <= m_atoms.data() + m_atoms.size());
    for (auto& info : res_info.atoms) {
      assert(info.residue == &res_info);
      static_cast<void>(info);
    }
    atom_count += res_info.atoms.size();
  }
  assert(m_atoms.size() == atom_count);
}
void Frame::reserve_molecules(size_t n) { m_molecules.reserve(n); }
void Frame::reserve_atoms(size_t n) {
  m_atoms.reserve(n);
  coordinates.reserve(n);
}
void Frame::reserve_residues(size_t n) { m_residues.reserve(n); }
XYZ& Frame::crd(BaseAtom& atom) {
  assert(m_atoms.data() <= &atom);
  assert(&atom <= m_atoms.data() + m_atoms.size());
  return coordinates[&atom - m_atoms.data()];
}
proxy::ProxySpan<proxy::Atom, BaseAtom> Frame::atoms() {
  return proxy::ProxySpan<proxy::Atom, BaseAtom>(m_atoms.data(), m_atoms.size());
}
proxy::ProxySpan<proxy::Residue, BaseResidue> Frame::residues() {
  return proxy::ProxySpan<proxy::Residue, BaseResidue>(m_residues.data(), m_residues.size());
}
proxy::ProxySpan<proxy::Molecule, BaseMolecule> Frame::molecules() {
  return proxy::ProxySpan<proxy::Molecule, BaseMolecule>(m_molecules.data(), m_molecules.size());
}
