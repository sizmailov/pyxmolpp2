#pragma once
#include "base.h"
#include "proxy-span.h"

namespace xmol::v1::proxy {

class Atom;
class Residue;

/**
 * Molecule, Residue and Atom classes are proxies (pointer wrappers) to corresponding underlying molecular data.
 * No ref counting/access validation actions performed. For ref counting counter parts see "references.h"
 *
 * Instances are invalidated on insertion/deletion of corresponding entity to parent frame.
 * Access to invalidated instance most likely would lead to immediate SEGFAULT (if you are lucky)
 *
 * */

/// Molecule proxy
class Molecule {
public:
  [[nodiscard]] const MoleculeName& name() const{
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->name;
  }

  void name(const MoleculeName& name){
    assert(m_molecule);
    assert(m_molecule->frame);
    m_molecule->name = name;
  }

  [[nodiscard]] bool empty() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_begin == m_molecule->residues.m_end;
  }
  [[nodiscard]] size_t size() const{
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_end - m_molecule->residues.m_begin;
  }

  Frame& frame() { return *m_molecule->frame; };
  ProxySpan<Residue, BaseResidue> residues(){
    return proxy::ProxySpan<proxy::Residue, BaseResidue>{m_molecule->residues};
  }
  ProxySpan<Atom, BaseAtom> atoms(){
    if (empty())
      return {};
    assert(m_molecule->residues.m_begin);
    assert(m_molecule->residues.m_end);
    return proxy::ProxySpan<proxy::Atom, BaseAtom>(m_molecule->residues.m_begin->atoms.m_begin,
                                                   (m_molecule->residues.m_begin + size() - 1)->atoms.m_end);
  }

  bool operator!=(const Molecule& rhs) const { return m_molecule != rhs.m_molecule; }
  Residue add_residue(const ResidueName& residueName, const ResidueId& residueId);

private:
  friend Atom;
  friend Residue;
  friend MoleculeRef;
  friend Frame;
  friend ProxySpan<Molecule, BaseMolecule>;
  BaseMolecule* m_molecule;
  explicit Molecule(BaseMolecule& molecule): m_molecule(&molecule) {};
  Molecule(BaseMolecule* ptr, BaseMolecule* end) : m_molecule(ptr) {};
  void advance() { ++m_molecule; };
  Molecule() = default; // constructs object in invalid state (with nullptrs)
};

/// Residue proxy
class Residue {
public:
  [[nodiscard]] const ResidueName& name() const {
    assert(m_residue);
    return m_residue->name;
  }
  void name(const ResidueName& name) {
    assert(m_residue);
    m_residue->name = name;
  }

  [[nodiscard]] bool empty() const {
    assert(m_residue);
    return m_residue->atoms.m_begin == m_residue->atoms.m_end;
  }

  [[nodiscard]] size_t size() const {
    assert(m_residue);
    return m_residue->atoms.m_end - m_residue->atoms.m_begin;
  }

  Molecule molecule()  { return Molecule(*m_residue->molecule); }
  Frame& frame()  { return *m_residue->molecule->frame; }

  ProxySpan<Atom, BaseAtom> atoms() { return ProxySpan<Atom, BaseAtom>{m_residue->atoms}; }

  bool operator!=(const Residue& rhs) const { return m_residue != rhs.m_residue; }
  Atom add_atom(const AtomName& atomName, const AtomId& atomId);

private:
  friend Atom;
  friend Molecule;
  friend Frame;
  friend ResidueRef;
  friend ProxySpan<Residue, BaseResidue>;
  explicit Residue(BaseResidue& residue) : m_residue(&residue){};
  BaseResidue* m_residue = nullptr;
  Residue(BaseResidue* ptr, BaseResidue* end) : m_residue(ptr){};
  void advance() { ++m_residue; }
  Residue() = default; // constructs object in invalid state (with nullptrs)
};

/// Atom proxy
class Atom {
public:
  Atom(const Atom& rhs) = default;
  Atom(Atom&& rhs) noexcept = default;
  Atom& operator=(const Atom& rhs) = default;
  Atom& operator=(Atom&& rhs) noexcept = default;

  [[nodiscard]] const AtomId& id() const { return m_atom->id; };
  void id(const AtomId& value) { m_atom->id = value; }

  [[nodiscard]] const AtomName& name() const { return m_atom->name; };
  void name(const AtomName& value) { m_atom->name = value; }

  [[nodiscard]] const XYZ& r() const { return *m_coords; }
  void r(const XYZ& value) { *m_coords = value; }

  Residue residue()  { return Residue(*m_atom->residue); }
  Molecule molecule() { return Molecule(*m_atom->residue->molecule); };
  Frame& frame() { return *m_atom->residue->molecule->frame; } ;

  bool operator!=(const Atom& rhs) const {
    return m_atom != rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }
  bool operator==(const Atom& rhs) const {
    return m_atom == rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }

protected:
  friend Frame;
  friend Residue;
  friend AtomRef;
  friend ProxySpan<Atom, BaseAtom>;
  explicit Atom(BaseAtom& atom);;
  XYZ* m_coords = nullptr;
  BaseAtom* m_atom = nullptr;

private:
  Atom(BaseAtom* ptr, BaseAtom* end);
  void advance() {
    ++m_atom;
    ++m_coords;
  }
  Atom() = default; // constructs object in invalid state (with nullptrs)
};
} // namespace xmol::v1::proxy