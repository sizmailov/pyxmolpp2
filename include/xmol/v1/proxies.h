#pragma once
#include "fwd.h"

namespace xmol::v1::proxy {

class Atom;
class Residue;

class Molecule {
public:
  [[nodiscard]] MoleculeName name() const;
  void name(const MoleculeName& name);

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;

  Frame& frame() noexcept;

private:
  friend Atom;
  friend Residue;
  friend MoleculeRef;
  friend Frame;
  explicit Molecule(BaseMolecule& molecule);
  BaseMolecule* m_molecule;
};

class Residue {
public:
  [[nodiscard]] ResidueName name() const;
  void name(const ResidueName& name);

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;

  Molecule molecule() noexcept;
  Frame& frame() noexcept;

private:
  friend Atom;
  friend Frame;
  friend ResidueRef;
  explicit Residue(BaseResidue& residue);
  BaseResidue* m_residue;
};

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

protected:
  friend Frame;
  friend AtomRef;
  Atom(BaseAtom& atom, XYZ& coords);
  XYZ* m_coords;
  BaseAtom* m_atom;
};
} // namespace xmol::v1::proxy