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
      throw DeadFrameAccessError(std::string("AtomSmartRef::") + func_name);
    }
  }
};

/// Smart Residue reference proxy
class ResidueSmartRef : public FrameObserver<ResidueSmartRef> {
public:
  ResidueSmartRef(ResidueRef&& res);

  /// Residue name
  [[nodiscard]] const ResidueName& name() const {
    check_precondition("name()");
    return m_ref.name();
  }
  void name(const ResidueName& name) {
    check_precondition("name()");
    m_ref.name(name);
  }

  /// Residue id
  [[nodiscard]] const ResidueId& id() const {
    check_precondition("id()");
    return m_ref.id();
  };
  void id(const ResidueId& value) {
    check_precondition("id()");
    m_ref.id(value);
  }

  /// Check if residue has no atoms
  [[nodiscard]] bool empty() const {
    check_precondition("empty()");
    return m_ref.empty();
  }

  /// Number of atoms in the residue
  [[nodiscard]] size_t size() const {
    check_precondition("size()");
    return m_ref.size();
  }

  /// Parent molecule
  MoleculeRef molecule() {
    check_precondition("molecule()");
    return m_ref.molecule();
  }

  /// Parent frame
  Frame& frame() {
    check_precondition("frame()");
    return m_ref.frame();
  }

  /// Atoms of the residue
  AtomRefSpan atoms() {
    check_precondition("atoms()");
    return m_ref.atoms();
  }

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const {
    check_precondition("operator!=()");
    return m_ref != rhs;
  }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  AtomRef add_atom(const AtomName& atomName, const AtomId& atomId) {
    check_precondition("add_atom()");
    return m_ref.add_atom(atomName, atomId);
  }

  operator ResidueRef&() {
    check_precondition("operator AtomRef&()");
    return m_ref;
  };

  operator const ResidueRef&() const {
    check_precondition("operator const AtomRef&()");
    return m_ref;
  };

private:
  ResidueRef m_ref;
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);

  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ResidueSmartRef::") + func_name);
    }
  }
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