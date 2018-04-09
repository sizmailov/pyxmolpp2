#include "xmol/polymer/Atom.h"

using namespace xmol::polymer;

// -------------------- Atom  -------------------------

const atomIndex_t& Atom::index() const{
  return m_index;
}


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
  m_index = rhs.m_index;
  m_chain = rhs.m_chain;
}

Residue::Residue(Residue&& rhs) noexcept : Container<Atom>(std::move(rhs)){
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  m_index = std::move(rhs.m_index);
  m_chain = std::move(rhs.m_chain);
}

Residue& Residue::operator=(const Residue& rhs) {
  Container<Atom>::operator=(rhs);
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = rhs.m_name;
  m_id = rhs.m_id;
  m_index = rhs.m_index;
  return *this;
}

Residue& Residue::operator=(Residue&& rhs) noexcept {
  Container<Atom>::operator=(std::move(rhs));
  for (auto& atom: this->elements){
    atom.value.m_residue = this;
  }
  m_name = std::move(rhs.m_name);
  m_id = std::move(rhs.m_id);
  m_index = std::move(rhs.m_index);
  return *this;
}

const residueIndex_t& Residue::index() const{
  return m_index;
}

const residueId_t& Residue::id() const {
  return m_id;
}

Residue& Residue::set_id(residueId_t&& value) {
  m_id = std::move(value);
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



Chain& Residue::chain() noexcept{
  return *m_chain;
}

const Chain& Residue::chain() const noexcept {
  return *m_chain;
}

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
  m_index = rhs.m_index;
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

const Frame& Chain::frame() const noexcept {
  return *m_frame;
}

// -------------------- Frame -------------------------


const frameIndex_t& Frame::index() const{
  return m_index;
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
        chain.index(),&chain,
        residue.index(),&residue,
        atom.index(),&atom);
  }
  auto compare_set(const Residue& residue){

    const Chain& chain = residue.chain();
    const Frame& frame = chain.frame();
    return std::tuple(
        frame.index(),&frame,
        chain.index(),&chain,
        residue.index(),&residue);
  }

  auto compare_set(const Chain& chain){
    const Frame& frame = chain.frame();
    return std::tuple(
        frame.index(),&frame,
        chain.index(),&chain);
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


