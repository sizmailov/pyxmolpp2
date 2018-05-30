#pragma once

#include "Atom_fwd.h"

#include "xmol/selection/ContainerSelection.h"
#include "xmol/pdb/PdbRecord_fwd.h"

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
  const Residue* parent() const;
  Residue* parent();

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

class Residue : public xmol::selection::Container<Atom>, public xmol::selection::ObservableBy<ElementReference<Atom>> {
public:
  Residue(const Residue& rhs);
  Residue(Residue&& rhs) noexcept;
  Residue& operator=(const Residue& rhs);
  Residue& operator=(Residue&& rhs) noexcept;
  ~Residue(){
    ObservableBy<ElementReference<Atom>>::notify_all(&ElementReference<Atom>::on_container_delete);
    ObservableBy<ElementReference<Atom>>::remove_all_observers();
  }

  const ResidueName& name() const;
  Residue& set_name(const ResidueName& value);

  const residueId_t& id() const;
  Residue& set_id(const residueId_t& value);

  const Chain& chain() const noexcept;
  Chain& chain() noexcept;

  const Residue* next() const noexcept;
  Residue* next() noexcept;

  const Residue* prev() const noexcept;
  Residue* prev() noexcept;

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

  const Chain* parent() const;
  Chain* parent();

private:
  Residue(Chain& chain, ResidueName name, residueId_t id, int reserve = 0);

  friend class xmol::selection::Container<Residue>;
  friend class Atom;
  friend class Chain;
  friend class ElementReference<Atom>;

  void add_reference(ElementReference<Atom>& aref){
    ObservableBy<ElementReference<Atom>>::add_observer(aref);
  }
  void remove_reference(ElementReference<Atom>& aref){
    ObservableBy<ElementReference<Atom>>::remove_observer(aref);
  }

  residueIndex_t pos_in_parent() const;

  ResidueName m_name;
  residueId_t m_id;
  Chain* m_chain;
  bool m_deleted = false;
};

class Chain : public xmol::selection::Container<Residue>, public xmol::selection::ObservableBy<ElementReference<Residue>> {
public:
  Chain(const Chain& rhs);
  Chain(Chain&& rhs) noexcept;
  Chain& operator=(const Chain& rhs);
  Chain& operator=(Chain&& rhs) noexcept;
  ~Chain(){
    ObservableBy<ElementReference<Residue>>::notify_all(&ElementReference<Residue>::on_container_delete);
    ObservableBy<ElementReference<Residue>>::remove_all_observers();
  }
  const chainIndex_t& index() const;

  const ChainName& name() const;
  Chain& set_name(const ChainName& value);

  const Frame& frame() const noexcept;
  Frame& frame() noexcept;

  Residue& emplace(ResidueName name, residueId_t id, int reserve = 0);
  Residue& emplace(const Residue& residue);

  Residue& operator[](const residueId_t& residueId);
  const Residue& operator[](const residueId_t& residueId) const;

  Residue& operator[](const residueSerial_t& residueId);
  const Residue& operator[](const residueSerial_t& residueId) const;

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

  const Frame* parent() const;
  Frame* parent();

private:
  Chain(Frame& frame, ChainName name, chainIndex_t id, int reserve = 0);

  friend class xmol::selection::Container<Chain>;

  friend class Frame;

  friend class Residue;

  friend class ElementReference<Residue>;


  void add_reference(ElementReference<Residue>& aref){
    ObservableBy<ElementReference<Residue>>::add_observer(aref);
  }
  void remove_reference(ElementReference<Residue>& aref){
    ObservableBy<ElementReference<Residue>>::remove_observer(aref);
  }

  chainIndex_t pos_in_parent() const;

  ChainName m_name;
  std::map<residueId_t, std::set<residueIndex_t>> m_lookup_table;
  chainIndex_t m_index;
  Frame* m_frame;
  bool m_deleted = false;
};

class Frame : public xmol::selection::Container<Chain> , public xmol::selection::ObservableBy<ElementReference<Chain>>{
public:
  explicit Frame(frameIndex_t id, int reserve = 0);

  Frame(const Frame& rhs);
  Frame(Frame&& rhs) noexcept;
  Frame& operator=(const Frame& rhs);
  Frame& operator=(Frame&& rhs) noexcept;
  ~Frame(){
    ObservableBy<ElementReference<Chain>>::notify_all(&ElementReference<Chain>::on_container_delete);
    ObservableBy<ElementReference<Chain>>::remove_all_observers();
  }
  const frameIndex_t& index() const;
  Frame& set_index(frameIndex_t index);

  void to_pdb(const std::string& filename) const;
  void to_pdb(const std::string& filename, const xmol::pdb::basic_PdbRecords& db) const;

  Chain& emplace(ChainName name, int reserve = 0);
  Chain& emplace(const Chain& chain);

  Chain& operator[](const ChainName& chainName);
  const Chain& operator[](const ChainName& chainName) const;

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
  friend class Chain;
  friend class ElementReference<Chain>;
  void add_reference(ElementReference<Chain>& aref){
    ObservableBy<ElementReference<Chain>>::add_observer(aref);
  }
  void remove_reference(ElementReference<Chain>& aref){
    ObservableBy<ElementReference<Chain>>::remove_observer(aref);
  }

  std::map<ChainName, std::set<chainIndex_t>> m_lookup_table;
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

using DeadAtomSelectionAccess = xmol::selection::dead_selection_access<Atom>;
using DeadResidueSelectionAccess = xmol::selection::dead_selection_access<Residue>;
using DeadChainSelectionAccess = xmol::selection::dead_selection_access<Chain>;

using DeletedAtomAccess = xmol::selection::deleted_element_access<Atom>;
using DeletedResidueAccess = xmol::selection::deleted_element_access<Residue>;
using DeletedChainAccess = xmol::selection::deleted_element_access<Chain>;

using OutOfRangeAtomSelection = xmol::selection::selection_out_of_range<Atom>;
using OutOfRangeResidueSelection = xmol::selection::selection_out_of_range<Residue>;
using OutOfRangeChainSelection= xmol::selection::selection_out_of_range<Chain>;

using AtomRef = ElementReference<Atom>;
using ResidueRef = ElementReference<Residue>;
using ChainRef = ElementReference<Chain>;

}
}
