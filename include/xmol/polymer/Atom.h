#pragma once

#include "Atom_fwd.h"
#include "xmol/selection/ContainerSelection.h"

#include <iostream>

namespace xmol::polymer {

class Atom {
public:

  Atom(const Atom& rhs) = default;
  Atom(Atom&& rhs) noexcept = default;
  Atom& operator=(const Atom& rhs) = default;
  Atom& operator=(Atom&& rhs) noexcept= default;

  const atomId_t& id() const;
  Atom& set_id(atomId_t&& value);

  const AtomName& name() const;
  Atom& set_name(AtomName&& value);

  const XYZ& r() const;
  Atom& set_r(XYZ&& value);

  const Residue& residue() const noexcept;
  Residue& residue() noexcept;

private:
  Atom(Residue& residue, AtomName name, atomId_t id, XYZ r);
  friend class xmol::selection::Container<Atom>;
  friend class Residue;

  XYZ m_r;
  Residue* m_residue;
  AtomName m_name;
  atomId_t m_id;
};

class Residue: public xmol::selection::Container<Atom> {
public:


  Residue(const Residue& rhs);
  Residue(Residue&& rhs) noexcept;
  Residue& operator=(const Residue& rhs);
  Residue& operator=(Residue&& rhs) noexcept ;

  const ResidueName& name() const;
  Residue& set_name(ResidueName&& value);

  const residueId_t& id() const;
  Residue& set_id(residueId_t&& value);

  const Chain& chain() const noexcept;
  Chain& chain() noexcept;

  Atom& emplace(AtomName name, atomId_t id, XYZ r);

  Atom& operator[](const AtomName& atomName);
  const Atom& operator[](const AtomName& atomName) const;

private:
  Residue(Chain& chain, ResidueName name, residueId_t id, int reserve=0);
  friend class xmol::selection::Container<Residue>;
  friend class Chain;
  ResidueName m_name;
  residueId_t m_id;
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

  Residue& emplace(ResidueName name, residueId_t id, int reserve=0);

  Residue& operator[](const residueId_t& residueId);
  const Residue& operator[](const residueId_t& residueId) const;

private:
  Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve=0);
  friend class xmol::selection::Container<Chain>;
  friend class Frame;
  friend class Residue;
  ChainName m_name;
  std::map<residueId_t, residueIndex_t> m_lookup_table;
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

  explicit Frame(frameIndex_t id, int reserve=0);

  Chain& emplace(ChainName name, int reserve=0);

  Chain& operator[](const chainIndex_t& chainIndex);
  const Chain& operator[](const chainIndex_t& residueId) const;

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


namespace xmol::selection {

template<typename T>
Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Residue>>
  SelectionBaseExtension<T,xmol::polymer::detail::enabled_if_atom<T>>::asResidues() const{
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Residue>> result;
  std::cout << "atoms as residues" << std::endl;
  return result;
};

}

