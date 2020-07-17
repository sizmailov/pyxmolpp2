#pragma once

#include "MoleculeRef.h"
#include "ResidueRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {
/// Smart Molecule reference proxy
class MoleculeSmartRef : public FrameObserver<MoleculeSmartRef> {
public:
  MoleculeSmartRef(MoleculeRef mol);

  [[nodiscard]] constexpr MoleculeName name() const {
    check_invariants("name");
    return m_ref.name();
  }

  template <int N>[[maybe_unused]] constexpr const MoleculeRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  template <int N>[[maybe_unused]] constexpr MoleculeRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] const MoleculeRef& name(const std::string& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] MoleculeRef name(const std::string& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr const MoleculeRef& name(const MoleculeName& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr MoleculeRef name(const MoleculeName& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return m_ref.empty();
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return m_ref.size();
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return m_ref.frame();
  }

  MoleculeIndex index() const {
    check_invariants("index");
    return m_ref.index();
  };

  /// Residues of the molecule
  ResidueSpan residues() const {
    check_invariants("residues");
    return m_ref.residues();
  }

  /// Atoms of the molecule
  AtomSpan atoms() const {
    check_invariants("atoms");
    return m_ref.atoms();
  }

  /// Atoms coordinates of the residue
  CoordSpan coords() const {
    check_invariants("coords");
    return m_ref.coords();
  }

  /// Check if references point to same data
  bool operator!=(const MoleculeRef& rhs) const {
    check_invariants("operator!=");
    return m_ref != rhs;
  }

  /// Check if references point to same data
  bool operator==(const MoleculeRef& rhs) const {
    check_invariants("operator!=");
    return m_ref == rhs;
  }

  /// Get children residue by id
  std::optional<ResidueRef> operator[](const ResidueId& id) const {
    check_invariants("operator[]");
    return m_ref[id];
  }

  /// Get children residue by id
  std::optional<ResidueRef> operator[](ResidueIdSerial id) const {
    check_invariants("operator[]");
    return m_ref[id];
  }

  /// @brief Adds residue to the end of the molecule and return its reference
  ///
  /// Invalidates all kinds of non-smart residue references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_residues() call prevents references invalidation
  ResidueRef add_residue() {
    check_invariants("add_residue");
    return m_ref.add_residue();
  }

  operator const MoleculeRef&() const& {
    check_invariants("operator MoleculeRef&");
    return m_ref;
  };

  operator MoleculeRef() const&& {
    check_invariants("operator const MoleculeRef&()");
    return m_ref;
  };

private:
  MoleculeRef m_ref;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("MoleculeSmartRef::") + func_name);
    }
  }
  friend Frame;
};
} // namespace xmol::proxy