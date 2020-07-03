#pragma once
#include "../proxy.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// Smart Atom reference proxy
class AtomSmartRef : public FrameObserver<AtomSmartRef> {
public:
  AtomSmartRef(AtomRef atom);

  /// Atom id
  [[nodiscard]] const AtomId& id() const {
    check_precondition("id()");
    return m_ref.id();
  };
  AtomRef& id(const AtomId& value) {
    check_precondition("id()");
    return m_ref.id(value);
  }

  /// Atom mass
  [[nodiscard]] float mass() const {
    check_precondition("mass()");
    return m_ref.mass();
  };
  AtomRef& mass(float value) {
    check_precondition("mass()");
    return m_ref.mass(value);
  }

  /// Van der Waals radius
  [[nodiscard]] float vdw_radius() const {
    check_precondition("vdw_radius()");
    return m_ref.vdw_radius();
  };
  AtomRef& vdw_radius(float value) {
    check_precondition("vdw_radius()");
    return m_ref.vdw_radius(value);
  }

  /// Atom name
  [[nodiscard]] const AtomName& name() const {
    check_precondition("name()");
    return m_ref.name();
  };
  AtomRef& name(const AtomName& value) {
    check_precondition("name()");
    return m_ref.name(value);
  }

  AtomRef& name(const char* value) {
    check_precondition("name()");
    return m_ref.name(value);
  }

  /// Atom coordinates
  [[nodiscard]] const XYZ& r() const {
    check_precondition("r()");
    return m_ref.r();
  }
  AtomRef& r(const XYZ& value) {
    check_precondition("r()");
    return m_ref.r(value);
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

  AtomIndex index() const {
    check_precondition("index()");
    return m_ref.index();
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
  ResidueSmartRef(ResidueRef res);

  /// Residue name
  [[nodiscard]] const ResidueName& name() const {
    check_precondition("name()");
    return m_ref.name();
  }
  ResidueRef& name(const ResidueName& name) {
    check_precondition("name()");
    return m_ref.name(name);
  }

  /// Residue id
  [[nodiscard]] const ResidueId& id() const {
    check_precondition("id()");
    return m_ref.id();
  };
  ResidueRef& id(const ResidueId& value) {
    check_precondition("id()");
    return m_ref.id(value);
  }

  ResidueRef& id(int value) {
    check_precondition("id()");
    return m_ref.id(value);
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

  ResidueIndex index() const{
    check_precondition("index()");
    return m_ref.index();
  };

  /// Atoms of the residue
  AtomSpan atoms() {
    check_precondition("atoms()");
    return m_ref.atoms();
  }

  /// Atoms coordinates of the residue
  CoordSpan coords() {
    check_precondition("coords()");
    return m_ref.coords();
  }

  /// Next residue in the molecule
  std::optional<ResidueRef> next() {
    check_precondition("next()");
    return m_ref.next();
  }

  /// Previous residue in the molecule
  std::optional<ResidueRef> prev() {
    check_precondition("prev()");
    return m_ref.prev();
  }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const AtomName& name){
    check_precondition("operator[]()");
    return m_ref[name];
  }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const char* name) {
    check_precondition("operator[]()");
    return m_ref[name];
  }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const std::string& name) {
    check_precondition("operator[]()");
    return m_ref[name];
  }

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const {
    check_precondition("operator!=()");
    return m_ref != rhs;
  }

  /// Check if references point to same data
  bool operator==(const ResidueRef& rhs) const {
    check_precondition("operator==()");
    return m_ref == rhs;
  }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  AtomRef add_atom() {
    check_precondition("add_atom()");
    return m_ref.add_atom();
  }

  operator ResidueRef&() {
    check_precondition("operator ResidueRef&()");
    return m_ref;
  };

  operator const ResidueRef&() const {
    check_precondition("operator const ResidueRef&()");
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
class MoleculeSmartRef : public FrameObserver<MoleculeSmartRef> {
public:
  MoleculeSmartRef(MoleculeRef mol);
  /// Molecule name
  [[nodiscard]] const MoleculeName& name() const {
    check_precondition("name()");
    return m_ref.name();
  }

  MoleculeRef& name(const MoleculeName& name) {
    check_precondition("name()");
    return m_ref.name(name);
  }

  /// Check if molecule has no residues
  [[nodiscard]] bool empty() const {
    check_precondition("empty()");
    return m_ref.empty();
  }

  /// Number of residues in molecule
  [[nodiscard]] size_t size() const {
    check_precondition("size()");
    return m_ref.size();
  }

  /// Parent frame
  Frame& frame() {
    check_precondition("frame()");
    return m_ref.frame();
  };

  MoleculeIndex index() const {
    check_precondition("index()");
    return m_ref.index();
  };

  /// Residues of the molecule
  ResidueSpan residues() {
    check_precondition("residues()");
    return m_ref.residues();
  }

  /// Atoms of the molecule
  AtomSpan atoms() {
    check_precondition("atoms()");
    return m_ref.atoms();
  }

  /// Atoms coordinates of the residue
  CoordSpan coords() {
    check_precondition("coords()");
    return m_ref.coords();
  }

  /// Check if references point to same data
  bool operator!=(const MoleculeRef& rhs) const {
    check_precondition("operator!=()");
    return m_ref != rhs;
  }

  /// Check if references point to same data
  bool operator==(const MoleculeRef& rhs) const {
    check_precondition("operator!=()");
    return m_ref == rhs;
  }

  /// Get children residue by id
  std::optional<ResidueRef> operator[](const ResidueId& id){
    check_precondition("operator[]()");
    return m_ref[id];
  }

  /// Get children residue by id
  std::optional<ResidueRef> operator[](residueSerial_t id) {
    check_precondition("operator[]()");
    return m_ref[id];
  }

  /// @brief Adds residue to the end of the molecule and return its reference
  ///
  /// Invalidates all kinds of non-smart residue references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_residues() call prevents references invalidation
  ResidueRef add_residue() {
    check_precondition("add_residue()");
    return m_ref.add_residue();
  }

  operator MoleculeRef&() {
    check_precondition("operator MoleculeRef&()");
    return m_ref;
  };

  operator const MoleculeRef&() const {
    check_precondition("operator const MoleculeRef&()");
    return m_ref;
  };

private:
  MoleculeRef m_ref;
  friend Frame;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("MoleculeSmartRef::") + func_name);
    }
  }
};

} // namespace xmol::proxy::smart