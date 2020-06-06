#pragma once
#include "../proxy.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// Smart Atom reference proxy
class AtomSmartRef : public FrameObserver<AtomSmartRef> {
public:
  AtomSmartRef(AtomRef&& atom);

  /// Atom id
  [[nodiscard]] const AtomId& id() const {
    check_precondition("id()");
    return m_ref.id();
  };
  void id(const AtomId& value) {
    check_precondition("id()");
    m_ref.id(value);
  }

  /// Atom name
  [[nodiscard]] const AtomName& name() const {
    check_precondition("name()");
    return m_ref.name();
  };
  void name(const AtomName& value) {
    check_precondition("name()");
    m_ref.name(value);
  }

  /// Atom coordinates
  [[nodiscard]] const XYZ& r() const {
    check_precondition("r()");
    return m_ref.r();
  }
  void r(const XYZ& value) {
    check_precondition("r()");
    m_ref.r(value);
  }

  /// Parent residue
  ResidueRef residue() {
    check_precondition("residue()");
    return m_ref.residue();
  }

  /// Parent molecule
  MoleculeRef molecule() {
    check_precondition("molecule()");
    return m_ref.molecule();
  };

  /// Parent frame
  Frame& frame() {
    check_precondition("frame()");
    return m_ref.frame();
  };

  /// Check if references point to same data
  bool operator!=(const AtomRef& rhs) const {
    check_precondition("operator!=()");
    return m_ref != rhs;
  }

  /// Check if references point to same data
  bool operator==(const AtomRef& rhs) const {
    check_precondition("operator==()");
    return m_ref == rhs;
  }

  operator AtomRef&() {
    check_precondition("operator const AtomRef&()");
    return m_ref;
  }
  operator const AtomRef&() const {
    check_precondition("operator const AtomRef&()");
    return m_ref;
  }

private:
  AtomRef m_ref;
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);

  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("AtomRef::") + func_name);
    }
  }
};

/// Smart Residue reference proxy
class ResidueSmartRef : public ResidueRef, public FrameObserver<ResidueSmartRef> {
public:
  ResidueSmartRef(ResidueRef&& res);

private:
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

/// Smart Molecule reference proxy
class MoleculeSmartRef : public MoleculeRef, public FrameObserver<MoleculeSmartRef> {
public:
  MoleculeSmartRef(MoleculeRef&& mol);

private:
  friend Frame;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};

} // namespace xmol::v1::proxy::smart