#include "xmol/polymer/Atom.h"

using namespace xmol::polymer;

// -------------------- Atom  -------------------------

const atomId_t& Atom::id() const {
  return m_id;
}

Atom& Atom::set_id(atomId_t&& value) {
  m_id = std::move(value);
  return *this;
}

const AtomName& Atom::name() const {
  return m_name;
}

Atom& Atom::set_name(AtomName&& value) {
  m_name = std::move(value);
  return *this;
}

const XYZ& Atom::r() const {
  return m_r;
}

Atom& Atom::set_r(XYZ&& value) {
  m_r = std::move(value);
  return *this;
}

// -------------------- Residue -------------------------

Residue::Residue(const Residue& rhs) : Container<Atom>(rhs){
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = rhs.m_name;
  m_id = rhs.m_id;
  m_chain = rhs.m_chain;
}

Residue::Residue(Residue&& rhs) noexcept : Container<Atom>(std::move(rhs)){
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  m_chain = std::move(rhs.m_chain);
}

Residue& Residue::operator=(const Residue& rhs) {
  Container<Atom>::operator=(rhs);
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = rhs.m_name;
  m_id = rhs.m_id;
  return *this;
}

Residue& Residue::operator=(Residue&& rhs) noexcept {
  Container<Atom>::operator=(std::move(rhs));
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  return *this;
}


const residueId_t& Residue::id() const {
  return m_id;
}

Residue& Residue::set_id(residueId_t&& value) {
  auto it = chain().m_lookup_table.find(m_id);
  auto index = it->second;
  m_id = std::move(value);
  chain().m_lookup_table[m_id] = index;
  return *this;
}

const ResidueName& Residue::name() const {
  return m_name;
}

Residue& Residue::set_name(ResidueName&& value) {
  m_name = std::move(value);
  return *this;
}

const Residue& Atom::residue() const noexcept{
  return *m_residue;
}

Residue& Atom::residue() noexcept {
  return *m_residue;
}

Atom::Atom(Residue& residue, AtomName name, atomId_t id,
    XYZ r) :
    m_r(r), m_name(std::move(name)), m_id(id), m_residue(&residue) {}


Chain& Residue::chain() noexcept{
  return *m_chain;
}

const Chain& Residue::chain() const noexcept {
  return *m_chain;
}

Atom& Residue::emplace(AtomName name, atomId_t id, XYZ r) {
  return Container<Atom>::emplace(*this,name,id, r);
}

Atom& Residue::operator[](const AtomName& atomName) {
  for(auto&a: this->elements){
    if (a.value.name()==atomName){
      return a.value;
    }
  }
  throw std::runtime_error("Residue has no atom "+atomName.str());
}

const Atom& Residue::operator[](const AtomName& atomName) const{
  // number of atoms in Residue usually is small
  // linear search outperforms hash tables
  for(auto&a: this->elements){
    if (a.value.name()==atomName){
      return a.value;
    }
  }
  throw std::runtime_error("Residue has no atom "+atomName.str());
}
Residue::Residue(Chain& chain, ResidueName name, residueId_t id, int reserve)  :
    Container<Atom>(reserve),
    m_name(std::move(name)), m_id(id), m_chain(&chain){}

// -------------------- Chain -------------------------


Chain::Chain(const Chain& rhs) : Container<Residue>(rhs){
  for (auto& residues: this->elements){
    residues.value.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
}

Chain::Chain(Chain&& rhs) noexcept : Container<Residue>(std::move(rhs)){
  for (auto& residues: this->elements){
    residues.value.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
}

Chain& Chain::operator=(const Chain& rhs) {
  Container<Residue>::operator=(rhs);
  for (auto& residues: this->elements){
    residues.value.m_chain = this;
  }
  m_name = rhs.m_name;
  m_frame = rhs.m_frame;
  return *this;
}

Chain& Chain::operator=(Chain&& rhs) noexcept {
  Container<Residue>::operator=(std::move(rhs));
  for (auto& residues: this->elements){
    residues.value.m_chain = this;
  }
  m_name = rhs.m_name;
  m_index = rhs.m_index;
  m_frame = rhs.m_frame;
  return *this;
}

const chainIndex_t& Chain::index() const{
  return m_index;
}

const ChainName& Chain::name() const {
  return m_name;
}

Chain& Chain::set_name(ChainName&& value) {
  m_name = std::move(value);
  return *this;
}

Frame& Chain::frame() noexcept{
  return *m_frame;
}

Residue& Chain::emplace(ResidueName name, residueId_t id, int reserve) {
    m_lookup_table.emplace(id,size());
    return Container<Residue>::emplace(*this, name, id, reserve);
}

const Residue& Chain::operator[](const residueId_t& residueId) const {
  return this->elements[m_lookup_table.at(residueId)].value;
}

Residue& Chain::operator[](const residueId_t& residueId) {
  return this->elements[m_lookup_table.at(residueId)].value;
}

const Frame& Chain::frame() const noexcept {
  return *m_frame;
}

Chain::Chain(Frame& frame, ChainName name, chainIndex_t id,
    int reserve)  :
    Container<Residue>(reserve),
    m_name(std::move(name)), m_index(id), m_frame(&frame){
}


// -------------------- Frame -------------------------


const frameIndex_t& Frame::index() const{
  return m_index;
}
Frame::Frame(frameIndex_t id, int reserve)  :
    Container<Chain>(reserve),
    m_index(id) {
}

Chain& Frame::emplace(ChainName name, int reserve) {
  return Container<Chain>::emplace(*this,name,chainIndex_t(size()),reserve);
}

Chain& Frame::operator[](const chainIndex_t& chainIndex) {
  if (chainIndex<0||chainIndex>=this->elements.size()){
    throw std::out_of_range("out_of_range Frame::opreator[]");
  }
  if (elements[chainIndex].is_deleted){
    throw std::runtime_error("Chain was deleted");
  }
  return elements[chainIndex].value;
}

const Chain& Frame::operator[](const chainIndex_t& chainIndex) const {
  if (chainIndex<0||chainIndex>=this->elements.size()){
    throw std::out_of_range("out_of_range Frame::opreator[]");
  }
  if (elements[chainIndex].is_deleted){
    throw std::runtime_error("Chain was deleted");
  }
  return elements[chainIndex].value;
}

Frame::Frame(const Frame& rhs) : Container<Chain>(rhs){
  for (auto& chain: this->elements){
    chain.value.m_frame = this;
  }
  m_index= rhs.m_index;
}

Frame::Frame(Frame&& rhs) noexcept : Container<Chain>(std::move(rhs)){
  for (auto& chain: this->elements){
    chain.value.m_frame = this;
  }
  m_index= rhs.m_index;
}

Frame& Frame::operator=(const Frame& rhs) {
  Container<Chain>::operator=(rhs);
  for (auto& chain: this->elements){
    chain.value.m_frame = this;
  }
  m_index= rhs.m_index;
  return *this;
}

Frame& Frame::operator=(Frame&& rhs) noexcept {
  Container<Chain>::operator=(std::move(rhs));
  for (auto& chain: this->elements){
    chain.value.m_frame = this;
  }
  m_index= rhs.m_index;
  return *this;
}
// -------------------- Stubs -------------------------

template<>
xmol::selection::Container<Atom>* xmol::selection::ElementWithFlags<Atom>::parent() noexcept {
  return &value.residue();
}

template<>
const xmol::selection::Container<Atom>* xmol::selection::ElementWithFlags<Atom>::parent() const noexcept {
  return &value.residue();
}

template<>
xmol::selection::Container<Residue>* xmol::selection::ElementWithFlags<Residue>::parent() noexcept {
  return &value.chain();
}

template<>
const xmol::selection::Container<Residue>* xmol::selection::ElementWithFlags<Residue>::parent() const noexcept {
  return &value.chain();
}

template<>
xmol::selection::Container<Chain>* xmol::selection::ElementWithFlags<Chain>::parent() noexcept {
  return &value.frame();
}

template<>
const xmol::selection::Container<Chain>* xmol::selection::ElementWithFlags<Chain>::parent() const noexcept {
  return &value.frame();
}


namespace {
  auto compare_set(const Atom& atom){
    const Residue& residue = atom.residue() ;
    const Chain& chain = residue.chain();
    const Frame& frame = chain.frame();
    return std::tuple(
        frame.index(),&frame,
        &chain,
        &residue,
        &atom);
  }
  auto compare_set(const Residue& residue){

    const Chain& chain = residue.chain();
    const Frame& frame = chain.frame();
    return std::tuple(
        frame.index(),&frame,
        &chain,
        &residue);
  }

  auto compare_set(const Chain& chain){
    const Frame& frame = chain.frame();
    return std::tuple(
        frame.index(),&frame,
        &chain);
  }
}
template<>
bool xmol::selection::ElementWithFlags<Atom>::operator<(const xmol::selection::ElementWithFlags<Atom>& rhs) const noexcept {
  return compare_set(this->value) < compare_set(rhs.value);
}

template<>
bool xmol::selection::ElementWithFlags<Residue>::operator<(const xmol::selection::ElementWithFlags<Residue>& rhs) const noexcept {
  return compare_set(this->value) < compare_set(rhs.value);
}

template<>
bool xmol::selection::ElementWithFlags<Chain>::operator<(const xmol::selection::ElementWithFlags<Chain>& rhs) const noexcept {
  return compare_set(this->value) < compare_set(rhs.value);
}


