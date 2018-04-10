#pragma once

#include "xmol/utils/ShortAsciiString.h"
#include "xmol/selection/ContainerSelection.h"

namespace xmol::polymer {

namespace detail{
  struct AtomNameTag{};
  struct ResidueNameTag{};
  struct ChainNameTag{};
}

using AtomName = xmol::utils::ShortAsciiString<4,detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3,detail::ResidueNameTag>;
using ChainName = xmol::utils::ShortAsciiString<1,detail::ChainNameTag>;


using atomIndex_t = int32_t;
using residueIndex_t = int32_t;
using chainIndex_t = int32_t;

using atomId_t = int32_t;
using residueId_t = int32_t;
using frameIndex_t = int32_t;


struct XYZ {
  double x,y,z;
};

class Atom;
class Residue;
class Chain;
class Frame;

class Atom {
public:

  Atom(const Atom& rhs) = default;
  Atom(Atom&& rhs) noexcept = default;
  Atom& operator=(const Atom& rhs) = default;
  Atom& operator=(Atom&& rhs) noexcept= default;

  const atomIndex_t& index() const;

  const atomId_t& id() const;
  Atom& set_id(atomId_t&& value);

  const AtomName& name() const;
  Atom& set_name(AtomName&& value);

  const XYZ& r() const;
  Atom& set_r(XYZ&& value);

  const Residue& residue() const noexcept;
  Residue& residue() noexcept;

private:
  Atom(Residue& residue, AtomName name, atomId_t id, XYZ r, atomIndex_t index) :
      m_r(r), m_name(std::move(name)), m_id(id), m_residue(&residue), m_index(index) {}
  friend class xmol::selection::Container<Atom>;
  friend class Residue;

  XYZ m_r;
  Residue* m_residue;
  AtomName m_name;
  atomId_t m_id;
  atomIndex_t m_index;
};

class Residue: public xmol::selection::Container<Atom> {
public:


  Residue(const Residue& rhs);
  Residue(Residue&& rhs) noexcept;
  Residue& operator=(const Residue& rhs);
  Residue& operator=(Residue&& rhs) noexcept ;


  const residueIndex_t& index() const;

  const ResidueName& name() const;
  Residue& set_name(ResidueName&& value);

  const residueId_t& id() const;
  Residue& set_id(residueId_t&& value);

  const Chain& chain() const noexcept;
  Chain& chain() noexcept;

  Atom& emplace(AtomName name, atomId_t id, XYZ r){
    return Container<Atom>::emplace(*this,name,id, r, residueIndex_t(size()));
  }

private:
  Residue(Chain& chain, ResidueName name, residueId_t id, residueIndex_t index, int reserve=0) :
      Container<Atom>(reserve),
      m_name(std::move(name)), m_id(id), m_chain(&chain), m_index(index){}
  friend class xmol::selection::Container<Residue>;
  friend class Chain;
  ResidueName m_name;
  residueId_t m_id;
  residueIndex_t m_index;
  Chain* m_chain;
};

class Chain: public xmol::selection::Container<Residue> {
public:

  Chain(const Chain& rhs);
  Chain(Chain&& rhs) noexcept;
  Chain& operator=(const Chain& rhs);
  Chain& operator=(Chain&& rhs) noexcept ;

  const chainIndex_t& index() const;

  const ChainName& name() const;
  Chain& set_name(ChainName&& value);

  const Frame& frame() const noexcept;
  Frame& frame() noexcept;

  Residue& emplace(ResidueName name, residueId_t id, int reserve=0){
    return Container<Residue>::emplace(*this,name,id, residueIndex_t(size()), reserve);
  }

private:
  Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve=0) :
      Container<Residue>(reserve),
      m_name(std::move(name)), m_index(id), m_frame(&frame){}
  friend class xmol::selection::Container<Chain>;
  friend class Frame;
  ChainName m_name;
  chainIndex_t m_index;
  Frame* m_frame;
};

class Frame: public xmol::selection::Container<Chain> {
public:
  Frame(const Frame& rhs);
  Frame(Frame&& rhs) noexcept;
  Frame& operator=(const Frame& rhs);
  Frame& operator=(Frame&& rhs) noexcept ;

  const frameIndex_t& index() const;

  explicit Frame(frameIndex_t id, int reserve=0) :
      Container<Chain>(reserve),
      m_index(id) {};

  Chain& emplace(ChainName name, int reserve=0){
    return Container<Chain>::emplace(*this,name,chainIndex_t(size()),reserve);
  }

private:
  frameIndex_t m_index;
};



inline bool operator==(const Atom& lhs, const Atom& rhs) noexcept {
  return &lhs == &rhs;
}
inline bool operator==(const Residue& lhs, const Residue& rhs) noexcept {
  return &lhs == &rhs;
}
inline bool operator==(const Chain& lhs, const Chain& rhs) noexcept {
  return &lhs == &rhs;
}
inline bool operator==(const Frame& lhs, const Frame& rhs) noexcept {
  return &lhs == &rhs;
}

inline bool operator!=(const Atom& lhs, const Atom& rhs) noexcept {
  return &lhs != &rhs;
}
inline bool operator!=(const Residue& lhs, const Residue& rhs) noexcept {
  return &lhs != &rhs;
}
inline bool operator!=(const Chain& lhs, const Chain& rhs) noexcept {
  return &lhs != &rhs;
}
inline bool operator!=(const Frame& lhs, const Frame& rhs) noexcept {
  return &lhs != &rhs;
}

}
