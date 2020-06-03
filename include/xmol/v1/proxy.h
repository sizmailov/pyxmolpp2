#pragma once
#include "fwd.h"
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
  [[nodiscard]] const MoleculeName& name() const;
  void name(const MoleculeName& name);

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;

  Frame& frame() noexcept;
  ProxySpan<Residue, BaseResidue> residues();
  ProxySpan<Atom, BaseAtom> atoms();

  bool operator!=(const Molecule& rhs) { return m_molecule != rhs.m_molecule; }
  Residue add_residue(const ResidueName& residueName, const ResidueId& residueId);
private:
  friend Atom;
  friend Residue;
  friend MoleculeRef;
  friend Frame;
  friend ProxySpan<Molecule, BaseMolecule>;
  explicit Molecule(BaseMolecule& molecule);
  BaseMolecule* m_molecule;
  Molecule() = default; // constructs object in invalid state (with nullptrs)
};

/// Residue proxy
class Residue {
public:
  [[nodiscard]] const ResidueName& name() const;
  void name(const ResidueName& name);

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;

  Molecule molecule() noexcept;
  Frame& frame() noexcept;

  ProxySpan<Atom, BaseAtom> atoms();

  bool operator!=(const Residue& rhs) { return m_residue != rhs.m_residue; }
  Atom add_atom(const AtomName& atomName, const AtomId& atomId);

private:
  friend Atom;
  friend Molecule;
  friend Frame;
  friend ResidueRef;
  friend ProxySpan<Residue, BaseResidue>;
  explicit Residue(BaseResidue& residue);
  BaseResidue* m_residue = nullptr;
  Residue() = default; // constructs object in invalid state (with nullptrs)
};

/// Atom proxy
class Atom {
public:
  Atom(const Atom& rhs) = default;
  Atom(Atom&& rhs) noexcept = default;
  Atom& operator=(const Atom& rhs) = default;
  Atom& operator=(Atom&& rhs) noexcept = default;

  [[nodiscard]] const AtomId& id() const;
  Atom& id(const AtomId& value);

  [[nodiscard]] const AtomName& name() const;
  Atom& name(const AtomName& value);

  [[nodiscard]] const XYZ& r() const { return *m_coords; }
  Atom& r(const XYZ& value);

  Residue residue() noexcept;
  Molecule molecule() noexcept;
  Frame& frame() noexcept;

  bool operator!=(const Atom& rhs) {
    return m_atom != rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }

protected:
  friend Frame;
  friend Residue;
  friend AtomRef;
  friend ProxySpan<Atom, BaseAtom>;
  explicit Atom(BaseAtom& atom);
  XYZ* m_coords = nullptr;
  BaseAtom* m_atom = nullptr;

private:
  Atom() = default; // constructs object in invalid state (with nullptrs)
};
} // namespace xmol::v1::proxy