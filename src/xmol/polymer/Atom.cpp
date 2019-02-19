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
  m_name = value;
  return *this;
}

const XYZ& Atom::r() const { return m_r; }

Atom& Atom::set_r(const XYZ& value) {
  m_r = value;
  return *this;
}

const Residue& Atom::residue() const noexcept {
  Ensures(m_residue != nullptr);
  Ensures(this >= m_residue->elements.data());
  Ensures(this < (m_residue->elements.data() + m_residue->elements.size()));
  return *m_residue;
}

Residue& Atom::residue() noexcept {
  Ensures(m_residue != nullptr);
  Ensures(this >= m_residue->elements.data());
  Ensures(this < (m_residue->elements.data() + m_residue->elements.size()));
  return *m_residue;
}

bool Atom::is_deleted() const { return m_deleted; }

void Atom::set_deleted() { m_deleted = true; }

Atom::Atom(Residue& residue, AtomName name, atomId_t id, XYZ r)
    : m_r(r), m_residue(&residue), m_name(std::move(name)), m_id(id) {}

// -------------------- Residue -------------------------

Residue::Residue(const Residue& rhs) :  Container<Atom>(rhs) {
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

Residue& Residue::set_id(const residueId_t& value) {
  auto it = chain().m_lookup_table.find(m_id);
  Ensures(it != chain().m_lookup_table.end());
  Ensures(!it->second.empty());
  m_id = value;
  auto index = pos_in_parent();
  it->second.erase(index);
  chain().m_lookup_table[value].insert(index);
  return *this;
}

const ResidueName& Residue::name() const { return m_name; }

Residue& Residue::set_name(const ResidueName& value) {
  m_name = value;
  return *this;
}

const Residue* Atom::parent() const { return m_residue; };
Residue* Atom::parent() { return m_residue; };

Chain& Residue::chain() noexcept {
  Ensures(m_chain != nullptr);
  Ensures(this >= m_chain->elements.data());
  Ensures(this < (m_chain->elements.data() + m_chain->elements.size()));
  return *m_chain;
}

const Chain& Residue::chain() const noexcept {
  Ensures(m_chain != nullptr);
  Ensures(this >= m_chain->elements.data());
  Ensures(this < (m_chain->elements.data() + m_chain->elements.size()));
  return *m_chain;
}

const Residue* Residue::next() const noexcept {
  if (this == &chain().elements.back() || (this + 1)->is_deleted()) {
    return nullptr;
  }
  return this + 1;
}

Residue* Residue::next() noexcept {
  if (this == &chain().elements.back() || (this + 1)->is_deleted()) {
    return nullptr;
  }
  return this + 1;
}

const Residue* Residue::prev() const noexcept {
  if (this == &chain().elements.front() || (this - 1)->is_deleted()) {
    return nullptr;
  }
  return this - 1;
}

Residue* Residue::prev() noexcept {
  if (this == &chain().elements.front() || (this - 1)->is_deleted()) {
    return nullptr;
  }
  return this - 1;
}

Atom& Residue::emplace(AtomName name, atomId_t id, XYZ r) {
  Atom* before = this->elements.data();
  auto& ref = Container<Atom>::emplace(*this, name, id, r);
  Atom* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Atom>>::notify_all(&ElementReference<Atom>::on_container_move, before, after);
  }
  return ref;
}

Atom& Residue::emplace(const Atom& atom) {
  Atom* before = this->elements.data();
  auto& a = Container<Atom>::emplace(Atom(atom));
  Atom* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Atom>>::notify_all(&ElementReference<Atom>::on_container_move, before, after);
  }
  a.m_residue = this;
  return a;
}

Atom& Residue::operator[](const AtomName& atomName) {
  for (auto& a : *this) {
    if (a.name() == atomName) {
      return a;
    }
  }
  throw OutOfRangeResidue("No atom with name `" + atomName.str() + "`");
}

const Atom& Residue::operator[](const AtomName& atomName) const {
  // number of atoms in Residue usually is small
  // linear search outperforms hash tables
  for (auto& a : *this) {
    if (a.name() == atomName) {
      return a;
    }
  }
  throw OutOfRangeResidue("No atom with name `" + atomName.str() + "`");
}

bool Residue::is_deleted() const { return m_deleted; }

void Residue::set_deleted() {
  Ensures(this >= chain().elements.data());
  Ensures(this < (chain().elements.data() + chain().elements.size()));
  for (Atom& a : *this) {
    a.set_deleted();
  };

  auto it = chain().m_lookup_table.find(m_id);

  Ensures(it != chain().m_lookup_table.end());
  Ensures(!it->second.empty());

  it->second.erase(pos_in_parent());
  m_deleted = true;
}

Residue::Residue(Chain& chain, ResidueName name, residueId_t id, int reserve)
    : Container<Atom>(reserve), m_name(std::move(name)), m_id(id), m_chain(&chain) {}

const Chain* Residue::parent() const { return m_chain; };
Chain* Residue::parent() { return m_chain; };

residueIndex_t Residue::pos_in_parent() const { return static_cast<residueIndex_t>(this - chain().elements.data()); }

// -------------------- Chain -------------------------

Chain::Chain(const Chain& rhs) : Container<Residue>(rhs) {
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
  m_lookup_table = rhs.m_lookup_table;
}

Chain::Chain(Chain&& rhs) noexcept : Container<Residue>(std::move(rhs)) {
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  m_deleted = rhs.m_deleted;
  m_lookup_table = std::move(rhs.m_lookup_table);
}

Chain& Chain::operator=(const Chain& rhs) {
  Container<Residue>::operator=(rhs);
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  set_name(rhs.m_name);
  m_index = rhs.m_index;
  m_deleted = rhs.m_deleted;
  m_lookup_table = rhs.m_lookup_table;
  return *this;
}

Chain& Chain::operator=(Chain&& rhs) noexcept {
  Container<Residue>::operator=(std::move(rhs));
  for (auto& residues : this->elements) {
    residues.m_chain = this;
  }
  set_name(rhs.m_name);
  m_index = rhs.m_index;
  m_deleted = rhs.m_deleted;
  m_lookup_table = std::move(rhs.m_lookup_table);
  return *this;
}

const chainIndex_t& Chain::index() const { return m_index; }

const ChainName& Chain::name() const { return m_name; }

Chain& Chain::set_name(const ChainName& value) {
  Ensures(this >= frame().elements.data());
  Ensures(this < (frame().elements.data() + frame().elements.size()));

  auto it = frame().m_lookup_table.find(m_name);
  Ensures(it != frame().m_lookup_table.end());
  Ensures(!it->second.empty());
  m_name = value;
  auto index = static_cast<int>(this - frame().elements.data());
  it->second.erase(index);
  frame().m_lookup_table[value].insert(index);
  return *this;
}

Frame& Chain::frame() noexcept { return *m_frame; }

Residue& Chain::emplace(ResidueName name, residueId_t id, int reserve) {
  m_lookup_table[id].insert(size());
  Residue* before = this->elements.data();
  auto& ref = Container<Residue>::emplace(*this, name, id, reserve);
  Residue* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Residue>>::notify_all(&ElementReference<Residue>::on_container_move, before, after);
  }
  return ref;
}

Residue& Chain::emplace(const Residue& residue) {
  m_lookup_table[residue.id()].insert(size());
  Residue* before = this->elements.data();
  auto& ref = Container<Residue>::emplace(Residue(residue));
  ref.m_chain = this;
  Residue* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Residue>>::notify_all(&ElementReference<Residue>::on_container_move, before, after);
  }
  return ref;
}

const Residue& Chain::operator[](const residueId_t& residueId) const {
  auto it = m_lookup_table.find(residueId);
  if (it == m_lookup_table.end() || it->second.empty()) {
    throw OutOfRangeChain("Chain has no residue with id `" + std::to_string(residueId.serial) + residueId.iCode.str() +
                          "`");
  }
  return this->elements[*it->second.begin()];
}

Residue& Chain::operator[](const residueId_t& residueId) {
  auto it = m_lookup_table.find(residueId);
  if (it == m_lookup_table.end() || it->second.empty()) {
    throw OutOfRangeChain("Chain has no residue with id `" + std::to_string(residueId.serial) + residueId.iCode.str() +
                          "`");
  }
  return this->elements[*it->second.begin()];
}

const Residue& Chain::operator[](const residueSerial_t& residueId) const {
  return this->operator[](ResidueId(residueId));
}

Residue& Chain::operator[](const residueSerial_t& residueId) { return this->operator[](ResidueId(residueId)); }

bool Chain::is_deleted() const { return m_deleted; }

void Chain::set_deleted() {
  asResidues().for_each([](Residue& r) { r.set_deleted(); });
  m_deleted = true;
}

const Frame& Chain::frame() const noexcept { return *m_frame; }

Chain::Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve)
    : Container<Residue>(reserve), m_name(std::move(name)), m_index(id), m_frame(&frame) {}

const Frame* Chain::parent() const { return m_frame; };
Frame* Chain::parent() { return m_frame; };

chainIndex_t Chain::pos_in_parent() const { return static_cast<chainIndex_t>(this - frame().elements.data()); }

// -------------------- Frame -------------------------

Frame::Frame(frameIndex_t id, int reserve) : Container<Chain>(reserve), m_index(id) {}

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

void Frame::to_pdb(const std::string& filename, const xmol::pdb::basic_PdbRecords& db) const {
  std::ofstream out(filename);
  if (out.fail()) {
    throw IOError("Can't open file `" + filename + "` for writing");
  }
  pdb::PdbWriter writer(out);
  writer.write(*this, db);
}

Chain& Frame::emplace(ChainName name, int reserve) {
  m_lookup_table[name].insert(static_cast<chainIndex_t>(elements.size()));
  Chain* before = this->elements.data();
  auto& ref = Container<Chain>::emplace(*this, name, chainIndex_t(size()), reserve);
  Chain* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Chain>>::notify_all(&ElementReference<Chain>::on_container_move, before, after);
  }
  return ref;
}

Chain& Frame::emplace(const Chain& chain) {
  m_lookup_table[chain.name()].insert(static_cast<chainIndex_t>(elements.size()));
  Chain* before = this->elements.data();
  auto& ref = Container<Chain>::emplace(Chain(chain));
  ref.m_frame = this;
  Chain* after = this->elements.data();
  if (before != after) {
    ObservableBy<ElementReference<Chain>>::notify_all(&ElementReference<Chain>::on_container_move, before, after);
  }
  return ref;
}

Chain& Frame::operator[](const ChainName& chainName) {
  auto it = m_lookup_table.find(chainName);
  if (it == m_lookup_table.end() || it->second.empty()) {
    throw OutOfRangeFrame("Frame has no chain named `" + chainName.str() + "`");
  }
  return elements[*it->second.begin()];
}

const Chain& Frame::operator[](const ChainName& chainName) const {
  auto it = m_lookup_table.find(chainName);
  if (it == m_lookup_table.end() || it->second.empty()) {
    throw OutOfRangeFrame("Frame has no chain named `" + chainName.str() + "`");
  }
  return elements[*it->second.begin()];
}

Frame::Frame(const Frame& rhs) : Container<Chain>(rhs) {
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  m_lookup_table = rhs.m_lookup_table;
}

Frame::Frame(Frame&& rhs) noexcept : Container<Chain>(std::move(rhs)) {
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  m_lookup_table = std::move(rhs.m_lookup_table);
}

Frame& Frame::operator=(const Frame& rhs) {
  Container<Chain>::operator=(rhs);
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  m_lookup_table = rhs.m_lookup_table;
  return *this;
}

Frame& Frame::operator=(Frame&& rhs) noexcept {
  Container<Chain>::operator=(std::move(rhs));
  for (auto& chain : this->elements) {
    chain.m_frame = this;
  }
  m_index = rhs.m_index;
  m_lookup_table = std::move(rhs.m_lookup_table);
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
std::tuple<frameIndex_t, const Frame*, const Chain*, const Residue*, const Atom*> compare_set(const Atom& atom) {
  const Residue& residue = atom.residue();
  const Chain& chain = residue.chain();
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain, &residue, &atom);
}
std::tuple<frameIndex_t, const Frame*, const Chain*, const Residue*> compare_set(const Residue& residue) {

  const Chain& chain = residue.chain();
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain, &residue);
}

std::tuple<frameIndex_t, const Frame*, const Chain*> compare_set(const Chain& chain) {
  const Frame& frame = chain.frame();
  return std::make_tuple(frame.index(), &frame, &chain);
}
}

namespace xmol {
namespace selection {

template <> bool xmol::selection::SelectionPointerComparator<Atom>::operator()(const Atom* lhs, const Atom* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}

template <>
bool xmol::selection::SelectionPointerComparator<Residue>::operator()(const Residue* lhs, const Residue* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}
template <>
bool xmol::selection::SelectionPointerComparator<Chain>::operator()(const Chain* lhs, const Chain* rhs) const {
  return compare_set(*lhs) < compare_set(*rhs);
}

template <typename T>
std::vector<XYZ> SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>::toCoords() const {
  std::vector<XYZ> result;
  result.reserve(this->size());

  for (auto& a : *this) {
    result.push_back(a.r());
  }
  return result;
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>::asResidues() const {
  using result_type = Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>;
  std::set<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>*> parents;
  for (auto& a : *this) {
    parents.insert(&a.residue());
  }
  return result_type(parents.begin(), parents.end());
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>::asChains() const {
  return asResidues().asChains();
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_residue<T>>::asChains() const {
  using result_type = Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>;
  std::set<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>*> parents;
  for (auto& a : *this) {
    parents.insert(&a.chain());
  }
  return result_type(parents.begin(), parents.end());
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_residue<T>>::asAtoms() const {
  using result_type = Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>;
  std::vector<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>*> result;
  for (auto& x : *this) {
    for (auto& y : x) {
      result.push_back(&y);
    }
  }
  return result_type(result.begin(), result.end(), xmol::selection::NoSortTag{});
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_chain<T>>::asResidues() const {
  using result_type = Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>;
  std::vector<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>*> result;
  for (auto& x : *this) {
    for (auto& y : x) {
      result.push_back(&y);
    }
  }
  return result_type(result.begin(), result.end(), xmol::selection::NoSortTag{});
};

template <typename T>
Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_chain<T>>::asAtoms() const {
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
