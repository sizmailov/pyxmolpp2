#include "xmol/Frame.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

using namespace xmol;
using namespace xmol::proxy::smart;

BaseResidue& Frame::add_residue(BaseMolecule& mol) {
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
                                           BaseResidue{{}, {}, {atoms_end, atoms_end}, &mol});
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
    notify_residues_move(old_begin, old_insert_pos, new_begin);
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

  notify_residues_move(old_insert_pos, old_end, new_inserted_pos + 1);
  // update pointers in atoms for shifted residues
  for (auto it = new_inserted_pos + 1; it != new_end; ++it) {
    for (auto& ait : it->atoms) {
      ait.residue = it;
    }
  }

  check_references_integrity();
  return *new_inserted_pos;
}

BaseAtom& Frame::add_atom(BaseResidue& residue) {
  assert(residue.molecule);
  assert(residue.molecule->frame == this);
  assert(m_residues.data() <= &residue);
  assert(&residue < m_residues.data() + m_residues.size());
  check_references_integrity();

  auto old_begin = m_atoms.data();
  auto old_end = m_atoms.data() + m_atoms.size();

  auto old_begin_crd = m_coordinates.data();
  auto old_end_crd = m_coordinates.data() + m_coordinates.size();

  auto old_insert_pos = residue.atoms.m_end;
  auto old_insert_crd_pos = old_begin_crd + (residue.atoms.m_end - old_begin);

  auto new_inserted_it = m_atoms.insert(m_atoms.begin() + (old_insert_pos - old_begin), BaseAtom{{}, {}, &residue});

  auto new_inserted_crd_it = m_coordinates.insert(m_coordinates.begin() + (old_insert_pos - old_begin), XYZ{});

  auto new_begin = m_atoms.data();
  auto new_begin_crd = m_coordinates.data();
  auto new_inserted_pos = new_begin + (new_inserted_it - m_atoms.begin());
  auto new_inserted_crd_pos = new_begin_crd + (new_inserted_crd_it - m_coordinates.begin());

  // update pointers in the residue & increase size
  residue.atoms.rebase(old_begin, new_begin);
  residue.atoms.m_end++;

  if (new_begin != old_begin) {
    // update pointers in residues before the residue
    for (auto& rInfo : future::Span{m_residues.data(), &residue}) {
      rInfo.atoms.rebase(old_begin, new_begin);
    }
    notify_atoms_move(old_begin, old_insert_pos, new_begin);
    notify_coordinates_move(old_begin_crd, old_insert_crd_pos, new_begin_crd);
  }

  // update pointers in residues after the residue
  for (auto& it : future::Span{&residue + 1, m_residues.data() + m_residues.size()}) {
    it.atoms.rebase(old_begin, new_begin + 1);
  }

  notify_atoms_move(old_insert_pos, old_end, new_inserted_pos + 1);
  notify_coordinates_move(old_insert_crd_pos, old_end_crd, new_inserted_crd_pos + 1);

  check_references_integrity();
  return *new_inserted_pos;
}

proxy::MoleculeRef Frame::add_molecule() {
  check_references_integrity();
  auto old_begin = m_molecules.data();
  auto old_end = old_begin + m_molecules.size();
  auto residues_end = m_residues.data() + m_residues.size();
  m_molecules.emplace_back(BaseMolecule{this, {}, {residues_end, residues_end}});
  auto new_begin = m_molecules.data();

  if (old_begin != new_begin) {
    notify_molecules_move(old_begin, old_end, new_begin);
    // update pointers in residues
    for (auto& info : m_residues) {
      info.molecule = new_begin + (info.molecule - old_begin);
    }
  }
  check_references_integrity();
  return proxy::MoleculeRef(m_molecules.back());
}
Frame& Frame::operator=(Frame&& other) {
  if (this != &other) {
    notify_frame_delete();
    utils::Observable<AtomSmartRef>::operator=(std::move(other));
    utils::Observable<ResidueSmartRef>::operator=(std::move(other));
    utils::Observable<MoleculeSmartRef>::operator=(std::move(other));
    utils::Observable<AtomSmartSelection>::operator=(std::move(other));
    utils::Observable<ResidueSmartSelection>::operator=(std::move(other));
    utils::Observable<MoleculeSmartSelection>::operator=(std::move(other));
    utils::Observable<AtomSmartSpan>::operator=(std::move(other));
    utils::Observable<ResidueSmartSpan>::operator=(std::move(other));
    utils::Observable<MoleculeSmartSpan>::operator=(std::move(other));
    utils::Observable<CoordSmartSpan>::operator=(std::move(other));
    utils::Observable<CoordSmartSelection>::operator=(std::move(other));
    notify_frame_moved(other);
    cell = std::move(other.cell);
    index = other.index;
    time = other.time;
    m_atoms = std::move(other.m_atoms);
    m_residues = std::move(other.m_residues);
    m_molecules = std::move(other.m_molecules);
    m_coordinates = std::move(other.m_coordinates);
    for (auto& mol : m_molecules) {
      mol.frame = this;
    }
  }
  check_references_integrity();
  other.check_references_integrity();
  return *this;
}

Frame& Frame::operator=(const Frame& other) {
  if (this != &other) {
    notify_frame_delete();
    cell = other.cell;
    index = other.index;
    time = other.time;
    m_atoms = other.m_atoms;
    m_residues = other.m_residues;
    m_molecules = other.m_molecules;
    m_coordinates = other.m_coordinates;
    for (auto& mol : m_molecules) {
      mol.frame = this;
      mol.residues.rebase(other.m_residues.data(), m_residues.data());
    }
    for (auto& res : m_residues) {
      res.molecule = m_molecules.data() + (res.molecule - other.m_molecules.data());
      res.atoms.rebase(other.m_atoms.data(), m_atoms.data());
    }
    for (auto& atom : m_atoms) {
      atom.residue = m_residues.data() + (atom.residue - other.m_residues.data());
    }
  }
  check_references_integrity();
  return *this;
}

Frame::Frame(const Frame& other)
    : cell(other.cell), index(other.index), time(other.time), m_atoms(other.m_atoms), m_residues(other.m_residues), m_molecules(other.m_molecules),
      m_coordinates(other.m_coordinates) {
  for (auto& mol : m_molecules) {
    mol.frame = this;
    mol.residues.rebase(other.m_residues.data(), m_residues.data());
  }
  for (auto& res : m_residues) {
    res.molecule = m_molecules.data() + (res.molecule - other.m_molecules.data());
    res.atoms.rebase(other.m_atoms.data(), m_atoms.data());
  }
  for (auto& atom : m_atoms) {
    atom.residue = m_residues.data() + (atom.residue - other.m_residues.data());
  }
  check_references_integrity();
}

Frame::Frame(Frame&& other)
    : utils::Observable<AtomSmartRef>(std::move(other)),
      utils::Observable<ResidueSmartRef>(std::move(other)),
      utils::Observable<MoleculeSmartRef>(std::move(other)),
      utils::Observable<AtomSmartSelection>(std::move(other)),
      utils::Observable<ResidueSmartSelection>(std::move(other)),
      utils::Observable<MoleculeSmartSelection>(std::move(other)),
      utils::Observable<AtomSmartSpan>(std::move(other)),
      utils::Observable<ResidueSmartSpan>(std::move(other)),
      utils::Observable<MoleculeSmartSpan>(std::move(other)),
      utils::Observable<CoordSmartSpan>(std::move(other)),
      utils::Observable<CoordSmartSelection>(std::move(other)),
      cell(std::move(other.cell)), index(other.index), time(other.time), m_atoms(std::move(other.m_atoms)), m_residues(std::move(other.m_residues)),
      m_molecules(std::move(other.m_molecules)), m_coordinates(std::move(other.m_coordinates)) {
  notify_frame_moved(other);
  for (auto& mol : m_molecules) {
    mol.frame = this;
  }
  check_references_integrity();
  other.check_references_integrity();
}
Frame::~Frame() { notify_frame_delete(); }

void Frame::check_references_integrity() {
#ifdef NDEBUG
  return; // disables check completely in release mode
#endif
  // disable except for smallest debug cases
  if (m_molecules.size() > 10 || m_residues.size() > 10 || m_atoms.size() > 10) {
    return;
  }

  auto mols = molecules();
  for (auto&& mol : mols) {
    assert(mol.size() == mol.residues().size());
    for (auto&& res : mol.residues()) {
      assert(res.size() == res.atoms().size());
      assert(res.molecule() == mol);
      for (auto&& atom : res.atoms()) {
        assert(atom.residue() == res);
        static_cast<void>(atom);
      }
    }
  }

  size_t res_count = 0;
  for (auto& mol : m_molecules) {
    assert(m_residues.data() <= mol.residues.m_begin);
    assert(mol.residues.m_begin <= mol.residues.m_end);
    assert(mol.residues.m_end <= m_residues.data() + m_residues.size());
    for (auto& info : mol.residues) {
      assert(info.molecule == &mol);
      static_cast<void>(info);
    }
    res_count += mol.residues.size();
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
void Frame::reserve_molecules(size_t n) {
  auto old_begin = m_molecules.data();
  auto old_end = m_molecules.data() + m_molecules.size();
  m_molecules.reserve(n);
  notify_molecules_move(old_begin, old_end, m_molecules.data());
}

void Frame::reserve_atoms(size_t n) {
  {
    auto old_begin = m_atoms.data();
    auto old_end = m_atoms.data() + m_atoms.size();
    m_atoms.reserve(n);
    notify_atoms_move(old_begin, old_end, m_atoms.data());
  }
  {
    auto old_begin = m_coordinates.data();
    auto old_end = m_coordinates.data() + m_coordinates.size();
    m_coordinates.reserve(n);
    notify_coordinates_move(old_begin, old_end, m_coordinates.data());
  }
}

void Frame::reserve_residues(size_t n) {
  auto old_begin = m_residues.data();
  auto old_end = m_residues.data() + m_residues.size();
  m_residues.reserve(n);
  notify_residues_move(old_begin, old_end, m_residues.data());
}

XYZ& Frame::crd(BaseAtom& atom) {
  assert(m_atoms.data() <= &atom);
  assert(&atom <= m_atoms.data() + m_atoms.size());
  return m_coordinates[&atom - m_atoms.data()];
}

proxy::AtomSpan Frame::atoms() { return proxy::AtomSpan(m_atoms.data(), m_atoms.size()); }
proxy::ResidueSpan Frame::residues() { return proxy::ResidueSpan(m_residues.data(), m_residues.size()); }
proxy::MoleculeSpan Frame::molecules() { return proxy::MoleculeSpan(m_molecules.data(), m_molecules.size()); }
proxy::CoordSpan Frame::coords() { return proxy::CoordSpan(*this, m_coordinates.data(), m_coordinates.size()); }

void Frame::notify_atoms_move(BaseAtom* old_begin, BaseAtom* old_end, BaseAtom* new_begin) const {
  if (old_begin != new_begin) {
    utils::Observable<AtomSmartRef>::notify(&AtomSmartRef::on_base_atoms_move, old_begin, old_end, new_begin);
    utils::Observable<AtomSmartSelection>::notify(&AtomSmartSelection::on_base_atoms_move, old_begin, old_end,
                                                      new_begin);
    utils::Observable<AtomSmartSpan>::notify(&AtomSmartSpan::on_base_atoms_move, old_begin, old_end, new_begin);
  }
}
void Frame::notify_residues_move(BaseResidue* old_begin, BaseResidue* old_end, BaseResidue* new_begin) const {
  if (old_begin != new_begin) {
    utils::Observable<ResidueSmartRef>::notify(&ResidueSmartRef::on_base_residues_move, old_begin, old_end,
                                                   new_begin);
    utils::Observable<ResidueSmartSelection>::notify(&ResidueSmartSelection::on_base_residues_move, old_begin,
                                                         old_end, new_begin);
    utils::Observable<ResidueSmartSpan>::notify(&ResidueSmartSpan::on_base_residues_move, old_begin, old_end,
                                                    new_begin);
  }
}
void Frame::notify_molecules_move(BaseMolecule* old_begin, BaseMolecule* old_end, BaseMolecule* new_begin) const {
  if (old_begin != new_begin) {
    utils::Observable<MoleculeSmartRef>::notify(&MoleculeSmartRef::on_base_molecules_move, old_begin, old_end,
                                                    new_begin);
    utils::Observable<MoleculeSmartSelection>::notify(&MoleculeSmartSelection::on_base_molecules_move, old_begin,
                                                          old_end, new_begin);
    utils::Observable<MoleculeSmartSpan>::notify(&MoleculeSmartSpan::on_base_molecules_move, old_begin, old_end,
                                                     new_begin);
  }
}
void Frame::notify_coordinates_move(XYZ* old_begin, XYZ* old_end, XYZ* new_begin) const {
  if (old_begin != new_begin) {
    utils::Observable<AtomSmartRef>::notify(&AtomSmartRef::on_coordinates_move, old_begin, old_end, new_begin);
    utils::Observable<AtomSmartSelection>::notify(&AtomSmartSelection::on_coordinates_move, old_begin, old_end,
                                                      new_begin);
    // AtomSmartSpan doesn't store coordinate references, nothing to update
    utils::Observable<CoordSmartSpan>::notify(&CoordSmartSpan::on_coordinates_move, old_begin, old_end, new_begin);
    utils::Observable<CoordSmartSelection>::notify(&CoordSmartSelection::on_coordinates_move, old_begin, old_end,
                                                       new_begin);
  }
}

void Frame::notify_frame_moved(Frame& other) {
  utils::Observable<AtomSmartRef>::notify(&AtomSmartRef::on_frame_move, other, *this);
  utils::Observable<ResidueSmartRef>::notify(&ResidueSmartRef::on_frame_move, other, *this);
  utils::Observable<MoleculeSmartRef>::notify(&MoleculeSmartRef::on_frame_move, other, *this);
  utils::Observable<AtomSmartSelection>::notify(&AtomSmartSelection::on_frame_move, other, *this);
  utils::Observable<ResidueSmartSelection>::notify(&ResidueSmartSelection::on_frame_move, other, *this);
  utils::Observable<MoleculeSmartSelection>::notify(&MoleculeSmartSelection::on_frame_move, other, *this);
  utils::Observable<AtomSmartSpan>::notify(&AtomSmartSpan::on_frame_move, other, *this);
  utils::Observable<ResidueSmartSpan>::notify(&ResidueSmartSpan::on_frame_move, other, *this);
  utils::Observable<MoleculeSmartSpan>::notify(&MoleculeSmartSpan::on_frame_move, other, *this);
  utils::Observable<CoordSmartSpan>::notify(&CoordSmartSpan::on_frame_move, other, *this);
  utils::Observable<CoordSmartSelection>::notify(&CoordSmartSelection::on_frame_move, other, *this);
}

void Frame::notify_frame_delete() const {
  utils::Observable<AtomSmartRef>::notify(&AtomSmartRef::on_frame_delete);
  utils::Observable<ResidueSmartRef>::notify(&ResidueSmartRef::on_frame_delete);
  utils::Observable<MoleculeSmartRef>::notify(&MoleculeSmartRef::on_frame_delete);
  utils::Observable<AtomSmartSelection>::notify(&AtomSmartSelection::on_frame_delete);
  utils::Observable<ResidueSmartSelection>::notify(&ResidueSmartSelection::on_frame_delete);
  utils::Observable<MoleculeSmartSelection>::notify(&MoleculeSmartSelection::on_frame_delete);
  utils::Observable<AtomSmartSpan>::notify(&AtomSmartSpan::on_frame_delete);
  utils::Observable<ResidueSmartSpan>::notify(&ResidueSmartSpan::on_frame_delete);
  utils::Observable<MoleculeSmartSpan>::notify(&MoleculeSmartSpan::on_frame_delete);
  utils::Observable<CoordSmartSpan>::notify(&CoordSmartSpan::on_frame_delete);
  utils::Observable<CoordSmartSelection>::notify(&CoordSmartSelection::on_frame_delete);
}
std::optional<proxy::MoleculeRef> Frame::operator[](const MoleculeName& name) {
  // vector outperforms any mapping, do simple first-match return;
  for (auto& mol: m_molecules){
    if (mol.name==name){
      return proxy::MoleculeRef(mol);
    }
  }
  return {};
}
std::optional<proxy::MoleculeRef> Frame::operator[](const char* name) {
  return operator[](MoleculeName(name));
}