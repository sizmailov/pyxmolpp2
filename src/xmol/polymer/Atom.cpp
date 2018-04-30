#include "xmol/polymer/Atom.h"
#include "xmol/pdb/PdbWriter.h"
#include "xmol/polymer/exceptions.h"
#include <fstream>

using namespace xmol::polymer;

// -------------------- Atom  -------------------------

const atomId_t& Atom::id() const { return m_id; }

Atom& Atom::set_id(atomId_t&& value) {
  m_id = std::move(value);
  return *this;
}

const AtomName& Atom::name() const { return m_name; }

Atom& Atom::set_name(const AtomName& value) {
  m_name = std::move(value);
  return *this;
}

const XYZ& Atom::r() const { return m_r; }

Atom& Atom::set_r(const XYZ& value) {
  m_r = std::move(value);
  return *this;
}

// -------------------- Residue -------------------------

Residue::Residue(const Residue& rhs) : Container<Atom>(rhs) {
  for (auto& atom : this->elements) {
    atom.m_residue = this;
  }
  m_name = rhs.m_name;
  m_id = rhs.m_id;
  m_chain = rhs.m_chain;
  m_deleted = rhs.m_deleted;
}

Residue::Residue(Residue&& rhs) noexcept : Container<Atom>(std::move(rhs)) {
  for (auto& atom : this->elements) {
    atom.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  m_chain = std::move(rhs.m_chain);
  m_deleted = rhs.m_deleted;
}

Residue& Residue::operator=(const Residue& rhs) {
  Container<Atom>::operator=(rhs);
  for (auto& atom : this->elements) {
    atom.m_residue = this;
  }
  m_name = rhs.m_name;
  m_id = rhs.m_id;
  m_deleted = rhs.m_deleted;
  return *this;
}

Residue& Residue::operator=(Residue&& rhs) noexcept {
  Container<Atom>::operator=(std::move(rhs));
  for (auto& atom : this->elements) {
    atom.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  m_deleted = rhs.m_deleted;
  return *this;
}

const residueId_t& Residue::id() const { return m_id; }

Residue& Residue::set_id(residueId_t&& value) {
  auto it = chain().m_lookup_table.find(m_id);
  auto index = it->second;
  m_id = std::move(value);
  chain().m_lookup_table[m_id] = index;
  return *this;
}

const ResidueName& Residue::name() const { return m_name; }

Residue& Residue::set_name(const ResidueName& value) {
  m_name = std::move(value);
  return *this;
}

const Residue& Atom::residue() const noexcept { return *m_residue; }

Residue& Atom::residue() noexcept { return *m_residue; }

bool Atom::is_deleted() const { return m_deleted; }

void Atom::set_deleted() { m_deleted = true; }

Atom::Atom(Residue& residue, AtomName name, atomId_t id, XYZ r)
    : m_r(r), m_name(std::move(name)), m_id(id), m_residue(&residue) {}

const xmol::selection::Container<Atom>* Atom::parent() const {
  return m_residue;
};
xmol::selection::Container<Atom>* Atom::parent() { return m_residue; };

Chain& Residue::chain() noexcept { return *m_chain; }

const Chain& Residue::chain() const noexcept { return *m_chain; }

Atom& Residue::emplace(AtomName name, atomId_t id, XYZ r) {
  return Container<Atom>::emplace(*this, name, id, r);
}

Atom& Residue::emplace(const Atom& atom) {
  auto& a = Container<Atom>::emplace(Atom(atom));
  a.m_residue = this;
  return a;
}

Atom& Residue::operator[](const AtomName& atomName) {
  for (auto& a : this->elements) {
    if (a.name() == atomName) {
      return a;
    }
  }
  throw OutOfRangeResidue("No atom with name `" + atomName.str()+"`");
}

const Atom& Residue::operator[](const AtomName& atomName) const {
  // number of atoms in Residue usually is small
  // linear search outperforms hash tables
  for (auto& a : this->elements) {
    if (a.name() == atomName) {
      return a;
    }
  }
  throw OutOfRangeResidue("No atom with name `" + atomName.str()+"`");
}

bool Residue::is_deleted() const { return m_deleted; }

void Residue::set_deleted() {
  asAtoms().for_each([](Atom& a) { a.set_deleted(); });
  m_deleted = true;
}

Residue::Residue(Chain& chain, ResidueName name, residueId_t id, int reserve)
    : Container<Atom>(reserve), m_name(std::move(name)), m_id(id),
      m_chain(&chain) {}

const xmol::selection::Container<Residue>* Residue::parent() const {
  return m_chain;
};
xmol::selection::Container<Residue>* Residue::parent() { return m_chain; };

// -------------------- Chain -------------------------

Chain::Chain(const Chain& rhs) : Container<Residue>(rhs) {
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
}

Chain::Chain(Chain&& rhs) noexcept : Container<Residue>(std::move(rhs)) {
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
}

Chain& Chain::operator=(const Chain& rhs) {
  Container<Residue>::operator=(rhs);
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
  return *this;
}

Chain& Chain::operator=(Chain&& rhs) noexcept {
  Container<Residue>::operator=(std::move(rhs));
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
  return *this;
}

const chainIndex_t& Chain::index() const { return m_index; }

const ChainName& Chain::name() const { return m_name; }

Chain& Chain::set_name(const ChainName& value) {
  m_name = std::move(value);
  return *this;
}

Frame& Chain::frame() noexcept { return *m_frame; }

Residue& Chain::emplace(ResidueName name, residueId_t id, int reserve) {
  m_lookup_table.emplace(id, size());
  return Container<Residue>::emplace(*this, name, id, reserve);
}

Residue& Chain::emplace(const Residue& residue) {
  m_lookup_table.emplace(residue.id(), size());
  auto& res = Container<Residue>::emplace(Residue(residue));
  res.m_chain = this;
  return res;
}

const Residue& Chain::operator[](const residueId_t& residueId) const {
  return this->elements[m_lookup_table.at(residueId)];
}

Residue& Chain::operator[](const residueId_t& residueId) {
  return this->elements[m_lookup_table.at(residueId)];
}

bool Chain::is_deleted() const { return m_deleted; }

void Chain::set_deleted() {
  asResidues().for_each([](Residue& r) { r.set_deleted(); });
  m_deleted = true;
}

const Frame& Chain::frame() const noexcept { return *m_frame; }

Chain::Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve)
    : Container<Residue>(reserve), m_name(std::move(name)), m_index(id),
      m_frame(&frame) {}

const xmol::selection::Container<Chain>* Chain::parent() const {
  return m_frame;
};
xmol::selection::Container<Chain>* Chain::parent() { return m_frame; };

// -------------------- Frame -------------------------

Frame::Frame(frameIndex_t id, int reserve)
    : Container<Chain>(reserve), m_index(id) {}

const frameIndex_t& Frame::index() const { return m_index; }

Frame& Frame::set_index(xmol::polymer::frameIndex_t index) {
  m_index = index;
  return *this;
}

void Frame::to_pdb(const std::string& filename) const {
  std::ofstream out(filename);
  if (out.fail()) {
    throw IOError("Can't open file `" + filename + "` for writing");
  }
  pdb::PdbWriter writer(out);
  writer.write(*this);
}

void Frame::to_pdb(const std::string& filename,
                   const xmol::pdb::basic_PdbRecords& db) const {
  std::ofstream out(filename);
  if (out.fail()) {
    throw IOError("Can't open file `" + filename + "` for writing");
  }
  pdb::PdbWriter writer(out);
  writer.write(*this, db);
}

Chain& Frame::emplace(ChainName name, int reserve) {
  return Container<Chain>::emplace(*this, name, chainIndex_t(size()), reserve);
}

Chain& Frame::emplace(const Chain& chain) {
  auto& c = Container<Chain>::emplace(Chain(chain));
  c.m_frame = this;
  return c;
}

Chain& Frame::operator[](const chainIndex_t& chainIndex) {
  if (chainIndex < 0 || chainIndex >= this->elements.size()) {
    throw OutOfRangeFrame("Chain index (="+std::to_string(chainIndex)+") is out of range");
  }
  if (elements[chainIndex].is_deleted()) {
    throw DeletedChainAccess("Chain (index="+std::to_string(chainIndex)+") was deleted");
  }
  return elements[chainIndex];
}

const Chain& Frame::operator[](const chainIndex_t& chainIndex) const {
  if (chainIndex < 0 || chainIndex >= this->elements.size()) {
    throw OutOfRangeFrame("Chain index (="+std::to_string(chainIndex)+") is out of range");
  }
  if (elements[chainIndex].is_deleted()) {
    throw DeletedChainAccess("Chain (index="+std::to_string(chainIndex)+") was deleted");
  }
  return elements[chainIndex];
}

Frame::Frame(const Frame& rhs) : Container<Chain>(rhs) {
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
}

Frame::Frame(Frame&& rhs) noexcept : Container<Chain>(std::move(rhs)) {
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
}

Frame& Frame::operator=(const Frame& rhs) {
  Container<Chain>::operator=(rhs);
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  return *this;
}

Frame& Frame::operator=(Frame&& rhs) noexcept {
  Container<Chain>::operator=(std::move(rhs));
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  return *this;
}
// -------------------- Stubs -------------------------
//
// template<typename T, >
// xmol::selection::Container<Atom>*
// xmol::selection::ElementFlags<Atom>::parent() noexcept {
//  return &residue();
//}
//
// template<>
// const xmol::selection::Container<const Atom>*
// xmol::selection::SelectionTraits<const Atom>::parent(const Atom& a) const
// noexcept {
//  return &a.residue();
//}
//
// template<>
// xmol::selection::Container<Residue>*
// xmol::selection::ElementFlags<Residue>::parent() noexcept {
//  return &value.chain();
//}
//
// template<>
// const xmol::selection::Container<Residue>*
// xmol::selection::ElementFlags<Residue>::parent() const noexcept {
//  return &value.chain();
//}
//
// template<>
// xmol::selection::Container<Chain>*
// xmol::selection::ElementFlags<Chain>::parent() noexcept {
//  return &value.frame();
//}
//
// template<>
// const xmol::selection::Container<Chain>*
// xmol::selection::ElementFlags<Chain>::parent() const noexcept {
//  return &value.frame();
//}

namespace {
auto compare_set(const Atom& atom) {
  const Residue& residue = atom.residue();
  const Chain& chain = residue.chain();
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain, &residue, &atom);
}
auto compare_set(const Residue& residue) {

  const Chain& chain = residue.chain();
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain, &residue);
}

auto compare_set(const Chain& chain) {
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain);
}
}

namespace xmol {
namespace selection {

template <>
bool xmol::selection::SelectionPointerComparator<Atom>::
operator()(const Atom* lhs, const Atom* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}

template <>
bool xmol::selection::SelectionPointerComparator<Residue>::
operator()(const Residue* lhs, const Residue* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}
template <>
bool xmol::selection::SelectionPointerComparator<Chain>::
operator()(const Chain* lhs, const Chain* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}

template <typename T>
std::vector<XYZ>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>::toCoords()
    const {
  std::vector<XYZ> result;
  result.reserve(this->size());

  for (auto& a : this->elements) {
    result.push_back(a->r());
  }
  return result;
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
SelectionBaseExtension<
    T, xmol::polymer::detail::enabled_if_atom<T>>::asResidues() const {
  using result_type = Selection<
      xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>;
  std::set<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>*>
      parents;
  for (auto& a : this->elements) {
    parents.insert(&a->residue());
  }
  return result_type(parents.begin(), parents.end());
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>::asChains()
    const {
  return asResidues().asChains();
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
SelectionBaseExtension<
    T, xmol::polymer::detail::enabled_if_residue<T>>::asChains() const {
  using result_type = Selection<
      xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>;
  std::set<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>*>
      parents;
  for (auto& a : this->elements) {
    parents.insert(&a->chain());
  }
  return result_type(parents.begin(), parents.end());
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
SelectionBaseExtension<
    T, xmol::polymer::detail::enabled_if_residue<T>>::asAtoms() const {
  using result_type = Selection<
      xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>;
  result_type result;
  for (auto& x : *this) {
    result += x.all();
  }
  return result;
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
SelectionBaseExtension<
    T, xmol::polymer::detail::enabled_if_chain<T>>::asResidues() const {
  using result_type = Selection<
      xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>;
  result_type result;
  for (auto& x : *this) {
    result += x.all();
  }
  return result;
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_chain<T>>::asAtoms()
    const {
  return asResidues().asAtoms();
};
}
}

template class xmol::selection::SelectionBaseExtension<Atom>;
template class xmol::selection::SelectionBaseExtension<const Atom>;

template class xmol::selection::SelectionBaseExtension<Residue>;
template class xmol::selection::SelectionBaseExtension<const Residue>;

template class xmol::selection::SelectionBaseExtension<Chain>;
template class xmol::selection::SelectionBaseExtension<const Chain>;
