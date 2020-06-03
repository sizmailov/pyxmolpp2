#pragma once
#include "../base.h"
#include "ProxySpan.h"
#include "Selection.h"

namespace xmol::v1 {
namespace proxy {

/**
 * Molecule, Residue and AtomRef classes are proxies (pointer wrappers) to corresponding underlying molecular data.
 * No ref counting/access validation actions performed. For ref counting counter parts see "references.h"
 *
 * Instances are invalidated on insertion/deletion of corresponding entity to parent frame.
 * Access to invalidated instance most likely would lead to immediate SEGFAULT (if you are lucky)
 *
 * */

/// Molecule proxy
class MoleculeRef {
public:
  [[nodiscard]] const MoleculeName& name() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->name;
  }

  void name(const MoleculeName& name) {
    assert(m_molecule);
    assert(m_molecule->frame);
    m_molecule->name = name;
  }

  [[nodiscard]] bool empty() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_begin == m_molecule->residues.m_end;
  }
  [[nodiscard]] size_t size() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_end - m_molecule->residues.m_begin;
  }

  Frame& frame() { return *m_molecule->frame; };
  ProxySpan<ResidueRef, BaseResidue> residues() {
    return proxy::ProxySpan<proxy::ResidueRef, BaseResidue>{m_molecule->residues};
  }
  ProxySpan<AtomRef, BaseAtom> atoms() {
    if (empty())
      return {};
    assert(m_molecule->residues.m_begin);
    assert(m_molecule->residues.m_end);
    return proxy::ProxySpan<proxy::AtomRef, BaseAtom>(m_molecule->residues.m_begin->atoms.m_begin,
                                                   (m_molecule->residues.m_begin + size() - 1)->atoms.m_end);
  }

  bool operator!=(const MoleculeRef& rhs) const { return m_molecule != rhs.m_molecule; }
  ResidueRef add_residue(const ResidueName& residueName, const ResidueId& residueId);

private:
  friend AtomRef;
  friend ResidueRef;
  friend smart::MoleculeSmartRef;
  friend Frame;
  friend ProxySpan<MoleculeRef, BaseMolecule>;
  friend Selection<MoleculeRef>::Comparator;
  BaseMolecule* m_molecule;
  explicit MoleculeRef(BaseMolecule& molecule) : m_molecule(&molecule){};
  MoleculeRef(BaseMolecule* ptr, BaseMolecule* end) : m_molecule(ptr){};
  void advance() { ++m_molecule; };
  MoleculeRef() = default; // constructs object in invalid state (with nullptrs)
};

/// Residue proxy
class ResidueRef {
public:
  [[nodiscard]] const ResidueName& name() const {
    assert(m_residue);
    return m_residue->name;
  }
  void name(const ResidueName& name) {
    assert(m_residue);
    m_residue->name = name;
  }

  [[nodiscard]] const ResidueId& id() const { return m_residue->id; };
  void id(const ResidueId& value) { m_residue->id = value; }

  [[nodiscard]] bool empty() const {
    assert(m_residue);
    return m_residue->atoms.m_begin == m_residue->atoms.m_end;
  }

  [[nodiscard]] size_t size() const {
    assert(m_residue);
    return m_residue->atoms.m_end - m_residue->atoms.m_begin;
  }

  MoleculeRef molecule() { return MoleculeRef(*m_residue->molecule); }
  Frame& frame() { return *m_residue->molecule->frame; }

  ProxySpan<AtomRef, BaseAtom> atoms() { return ProxySpan<AtomRef, BaseAtom>{m_residue->atoms}; }

  bool operator!=(const ResidueRef& rhs) const { return m_residue != rhs.m_residue; }
  AtomRef add_atom(const AtomName& atomName, const AtomId& atomId);

private:
  friend AtomRef;
  friend MoleculeRef;
  friend Frame;
  friend smart::ResidueSmartRef;
  friend ProxySpan<ResidueRef, BaseResidue>;
  friend Selection<ResidueRef>::Comparator;
  explicit ResidueRef(BaseResidue& residue) : m_residue(&residue){};
  BaseResidue* m_residue = nullptr;
  ResidueRef(BaseResidue* ptr, BaseResidue* end) : m_residue(ptr){};
  void advance() { ++m_residue; }
  ResidueRef() = default; // constructs object in invalid state (with nullptrs)
};

/// AtomRef proxy
class AtomRef {
public:
  AtomRef(const AtomRef& rhs) = default;
  AtomRef(AtomRef&& rhs) noexcept = default;
  AtomRef& operator=(const AtomRef& rhs) = default;
  AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  [[nodiscard]] const AtomId& id() const { return m_atom->id; };
  void id(const AtomId& value) { m_atom->id = value; }

  [[nodiscard]] const AtomName& name() const { return m_atom->name; };
  void name(const AtomName& value) { m_atom->name = value; }

  [[nodiscard]] const XYZ& r() const { return *m_coords; }
  void r(const XYZ& value) { *m_coords = value; }

  ResidueRef residue() { return ResidueRef(*m_atom->residue); }
  MoleculeRef molecule() { return MoleculeRef(*m_atom->residue->molecule); };
  Frame& frame() { return *m_atom->residue->molecule->frame; };

  bool operator!=(const AtomRef& rhs) const {
    return m_atom != rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }
  bool operator==(const AtomRef& rhs) const {
    return m_atom == rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }

protected:
  friend Frame;
  friend ResidueRef;
  friend smart::AtomSmartRef;
  friend ProxySpan<AtomRef, BaseAtom>;
  friend Selection<proxy::AtomRef>::Comparator;
  explicit AtomRef(BaseAtom& atom);
  XYZ* m_coords = nullptr;
  BaseAtom* m_atom = nullptr;

private:
  AtomRef(BaseAtom* ptr, BaseAtom* end);
  void advance() {
    ++m_atom;
    ++m_coords;
  }
  AtomRef() = default; // constructs object in invalid state (with nullptrs)
};
} // namespace proxy

template <>
struct Selection<proxy::AtomRef>::Comparator {
  bool operator()(const proxy::AtomRef& p1, const proxy::AtomRef& p2){
    return p1.m_atom < p2.m_atom;
  }
};

template <>
struct Selection<proxy::ResidueRef>::Comparator {
  bool operator()(const proxy::ResidueRef& p1, const proxy::ResidueRef& p2){
    return p1.m_residue < p2.m_residue;
  }
};

template <>
struct Selection<proxy::MoleculeRef>::Comparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2){
    return p1.m_molecule < p2.m_molecule;
  }
};

} // namespace xmol::v1