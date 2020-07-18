#pragma once

#include "xmol/base.h"

namespace xmol::proxy::api {

template <typename Derived, typename MRef> class ConstMoleculeAPI {
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:
  [[nodiscard]] constexpr MoleculeName name() const {
    derived().check_invariants("name");
    return derived().mol_ptr()->name;
  }

  [[nodiscard]] constexpr bool empty() const {
    derived().check_invariants("empty");
    return derived().mol_ptr()->residues.m_begin == derived().mol_ptr()->residues.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    derived().check_invariants("size");
    return derived().mol_ptr()->residues.m_end - derived().mol_ptr()->residues.m_begin;
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().mol_ptr()->frame;
  }

  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const;

  // TODO: enable
  //
  //  /// Residues of the molecule
  //  [[nodiscard]] ConstResidueSpan residues() const {
  //    derived().check_invariants("residues");
  //    return ResidueSpan{derived().mol_ptr()->residues};
  //  }
  //
  //  /// Atoms of the molecule
  //  [[nodiscard]] ConstAtomSpan atoms() const {
  //    derived().check_invariants("atoms");
  //    if (empty())
  //      return {};
  //    assert(derived().mol_ptr()->residues.m_begin);
  //    assert(derived().mol_ptr()->residues.m_end);
  //    return ConstAtomSpan(derived().mol_ptr()->residues.m_begin->atoms.m_begin,
  //                    (derived().mol_ptr()->residues.m_begin + size() - 1)->atoms.m_end);
  //  }
  //
  //  /// Atom coordinates of the molecule
  //  [[nodiscard]] ConstCoordSpan coords() const {
  //    derived().check_invariants("coords");
  //    return atoms().coords();
  //  }

  std::optional<ConstResidueRef> operator[](const ResidueId& id) const;
  std::optional<ConstResidueRef> operator[](ResidueIdSerial id) const;

  /// Check if references point to same data
  constexpr bool operator!=(const ConstMoleculeAPI& rhs) const {
    derived().check_invariants("operator!=");
    return derived().mol_ptr() != rhs.derived().mol_ptr();
  }
  constexpr bool operator==(const ConstMoleculeAPI& rhs) const {
    derived().check_invariants("operator==");
    return derived().mol_ptr() == rhs.derived().mol_ptr();
  }
};

template <typename Derived, typename MRef> class MoleculeAPI : public ConstMoleculeAPI<Derived> {
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:

  using ConstMoleculeAPI<Derived>::name;

  template <int N>[[maybe_unused]] constexpr const MRef& name(const char (&value)[N]) const& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = MoleculeName(value);
    return static_cast<const MRef&>(derived());
    ;
  }

  template <int N>[[maybe_unused]] constexpr MRef name(const char (&value)[N]) const&& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = MoleculeName(value);
    return static_cast<const MRef&>(derived());
    ;
  }

  [[maybe_unused]] const MRef& name(const std::string& value) const& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = MoleculeName(value);
    return static_cast<const MRef&>(derived());
    ;
  }

  [[maybe_unused]] MRef name(const std::string& value) const&& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = MoleculeName(value);
    return static_cast<const MRef&>(derived());
    ;
  }

  [[maybe_unused]] constexpr const MRef& name(const MoleculeName& value) const& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = value;
    return static_cast<const MRef&>(derived());
    ;
  }

  [[maybe_unused]] constexpr MRef name(const MoleculeName& value) const&& {
    derived().check_invariants("name");
    derived().mol_ptr()->name = value;
    return static_cast<const MRef&>(derived());
    ;
  }

  [[nodiscard]] constexpr Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().mol_ptr()->frame;
  }

  /// Residues of the molecule
  [[nodiscard]] ResidueSpan residues() const {
    derived().check_invariants("residues");
    return ResidueSpan{derived().mol_ptr()->residues};
  }

  /// Atoms of the molecule
  [[nodiscard]] AtomSpan atoms() const {
    derived().check_invariants("atoms");
    if (derived().empty())
      return {};
    assert(derived().mol_ptr()->residues.m_begin);
    assert(derived().mol_ptr()->residues.m_end);
    return AtomSpan(derived().mol_ptr()->residues.m_begin->atoms.m_begin,
                    (derived().mol_ptr()->residues.m_begin + derived().size() - 1)->atoms.m_end);
  }

  /// Atom coordinates of the molecule
  [[nodiscard]] CoordSpan coords() const {
    derived().check_invariants("coords");
    return atoms().coords();
  }

  std::optional<ResidueRef> operator[](const ResidueId& id) const;
  std::optional<ResidueRef> operator[](ResidueIdSerial id) const;

  /// @brief Adds residue to the end of the molecule and return its reference
  ///
  /// Invalidates all kinds of non-smart residue references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_residues() call prevents references invalidation
  [[nodiscard]] ResidueRef add_residue() const;
};

} // namespace xmol::proxy::api