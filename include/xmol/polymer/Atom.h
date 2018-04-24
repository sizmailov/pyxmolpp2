#pragma once

#include "Atom_fwd.h"
#include "xmol/selection/ContainerSelection.h"

#include <iostream>

namespace xmol {
namespace polymer {

class Atom {
public:
  Atom(const Atom& rhs) = default;
  Atom(Atom&& rhs) noexcept = default;
  Atom& operator=(const Atom& rhs) = default;
  Atom& operator=(Atom&& rhs) noexcept = default;

  const atomId_t& id() const;
  Atom& set_id(atomId_t&& value);

  const AtomName& name() const;
  Atom& set_name(const AtomName& value);

  const XYZ& r() const;
  Atom& set_r(const XYZ& value);

  const Residue& residue() const noexcept;
  Residue& residue() noexcept;

  bool is_deleted() const;
  void set_deleted();
  const xmol::selection::Container<Atom>* parent() const;
  xmol::selection::Container<Atom>* parent();

private:
  Atom(Residue& residue, AtomName name, atomId_t id, XYZ r);

  friend class xmol::selection::Container<Atom>;

  friend class Residue;

  XYZ m_r;
  Residue* m_residue;
  AtomName m_name;
  atomId_t m_id;
  bool m_deleted = false;
};

class Residue : public xmol::selection::Container<Atom> {
public:
  Residue(const Residue& rhs);
  Residue(Residue&& rhs) noexcept;
  Residue& operator=(const Residue& rhs);
  Residue& operator=(Residue&& rhs) noexcept;

  const ResidueName& name() const;
  Residue& set_name(const ResidueName& value);

  const residueId_t& id() const;
  Residue& set_id(residueId_t&& value);

  const Chain& chain() const noexcept;
  Chain& chain() noexcept;

  Atom& emplace(AtomName name, atomId_t id, XYZ r);
  Atom& emplace(const Atom& atom);

  Atom& operator[](const AtomName& atomName);
  const Atom& operator[](const AtomName& atomName) const;

  xmol::selection::Selection<const Atom> asAtoms() const {
    return all();
  }

  xmol::selection::Selection<Atom> asAtoms() {
    return all();
  }

  bool is_deleted() const;
  void set_deleted();

  const xmol::selection::Container<Residue>* parent() const;
  xmol::selection::Container<Residue>* parent();

private:
  Residue(Chain& chain, ResidueName name, residueId_t id, int reserve = 0);

  friend class xmol::selection::Container<Residue>;

  friend class Chain;

  ResidueName m_name;
  residueId_t m_id;
  Chain* m_chain;
  bool m_deleted = false;
};

class Chain : public xmol::selection::Container<Residue> {
public:
  Chain(const Chain& rhs);
  Chain(Chain&& rhs) noexcept;
  Chain& operator=(const Chain& rhs);
  Chain& operator=(Chain&& rhs) noexcept;

  const chainIndex_t& index() const;

  const ChainName& name() const;
  Chain& set_name(const ChainName& value);

  const Frame& frame() const noexcept;
  Frame& frame() noexcept;

  Residue& emplace(ResidueName name, residueId_t id, int reserve = 0);
  Residue& emplace(const Residue& residue);

  Residue& operator[](const residueId_t& residueId);
  const Residue& operator[](const residueId_t& residueId) const;

  xmol::selection::Selection<const Atom> asAtoms() const {
    return all().asAtoms();
  }

  xmol::selection::Selection<Atom> asAtoms() {
    return all().asAtoms();
  }

  xmol::selection::Selection<const Residue> asResidues() const {
    return all();
  }

  xmol::selection::Selection<Residue> asResidues() {
    return all();
  }

  bool is_deleted() const;
  void set_deleted();

  const xmol::selection::Container<Chain>* parent() const;
  xmol::selection::Container<Chain>* parent();

private:
  Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve = 0);

  friend class xmol::selection::Container<Chain>;

  friend class Frame;

  friend class Residue;

  ChainName m_name;
  std::map<residueId_t, residueIndex_t> m_lookup_table;
  chainIndex_t m_index;
  Frame* m_frame;
  bool m_deleted = false;
};

class Frame : public xmol::selection::Container<Chain> {
public:
  explicit Frame(frameIndex_t id, int reserve = 0);

  Frame(const Frame& rhs);
  Frame(Frame&& rhs) noexcept;
  Frame& operator=(const Frame& rhs);
  Frame& operator=(Frame&& rhs) noexcept;

  const frameIndex_t& index() const;
  Frame& set_index(frameIndex_t index);

  Chain& emplace(ChainName name, int reserve = 0);
  Chain& emplace(const Chain& chain);

  Chain& operator[](const chainIndex_t& chainIndex);
  const Chain& operator[](const chainIndex_t& residueId) const;

  xmol::selection::Selection<const Atom> asAtoms() const {
    return asResidues().asAtoms();
  }

  xmol::selection::Selection<Atom> asAtoms() {
    return asResidues().asAtoms();
  }

  xmol::selection::Selection<const Residue> asResidues() const {
    return asChains().asResidues();
  }

  xmol::selection::Selection<Residue> asResidues() {
    return asChains().asResidues();
  }

  xmol::selection::Selection<const Chain> asChains() const {
    return all();
  }

  xmol::selection::Selection<Chain> asChains() {
    return all();
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

using ConstAtomSelection = xmol::selection::Selection<const Atom>;
using AtomSelection = xmol::selection::Selection<Atom>;

using ConstResidueSelection = xmol::selection::Selection<const Residue>;
using ResidueSelection = xmol::selection::Selection<Residue>;

using ConstChainSelection = xmol::selection::Selection<const Chain>;
using ChainSelection = xmol::selection::Selection<Chain>;
}
}