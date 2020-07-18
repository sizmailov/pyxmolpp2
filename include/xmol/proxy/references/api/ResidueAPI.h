#pragma once

#include "xmol/base.h"
#include "xmol/proxy/references/MoleculeRef.h"

namespace xmol::proxy::api {

template <typename Derived, typename RRef> class ConstResidueAPI {
private:
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:
  [[nodiscard]] constexpr ResidueId id() const {
    derived().check_invariants("id");
    return derived().res_ptr()->id;
  }

  [[nodiscard]] constexpr ResidueName name() const {
    derived().check_invariants("name");
    return derived().res_ptr()->name;
  }

  [[nodiscard]] ResidueIndex index() const;

  [[nodiscard]] constexpr bool empty() const {
    derived().check_invariants("empty");
    return derived().res_ptr()->atoms.m_begin == derived().res_ptr()->atoms.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    derived().check_invariants("size");
    return derived().res_ptr()->atoms.m_end - derived().res_ptr()->atoms.m_begin;
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<ConstAtomRef> operator[](const AtomName& name) const;
  [[nodiscard]] std::optional<ConstAtomRef> operator[](const char* name) const;
  [[nodiscard]] std::optional<ConstAtomRef> operator[](const std::string& name) const;

  /// Next residue in the molecule
  [[nodiscard]] std::optional<RRef> next() const {
    if (derived().res_ptr() + 1 < derived().res_ptr()->molecule->residues.begin() + molecule().size()) {
      return RRef(*(derived().res_ptr() + 1));
    }
    return std::nullopt;
  }

  /// Previous residue in the molecule
  [[nodiscard]] std::optional<RRef> prev() const {
    if (derived().res_ptr() >= derived().res_ptr()->molecule->residues.begin() + 1) {
      return RRef(*(derived().res_ptr() - 1));
    }
    return std::nullopt;
  }

//  /// Atoms of the residue
//  [[nodiscard]] ConstAtomSpan atoms() const { return ConstAtomSpan{derived().res_ptr()->atoms}; }
//
//  /// Atom coordinates of the molecule
//  [[nodiscard]] ConstCoordSpan coords() const { return atoms().coords(); }


  [[nodiscard]] constexpr ConstMoleculeRef molecule() const {
    derived().check_invariants("molecule");
    return ConstMoleculeRef(*derived().res_ptr()->molecule);
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().res_ptr()->molecule->frame;
  }

  /// Check if references point to same data
  [[nodiscard]] constexpr bool operator!=(const ConstResidueAPI& rhs) const {
    derived().check_invariants("operator!=");
    return derived().res_ptr() != rhs.derived().res_ptr();
  }

  [[nodiscard]] constexpr bool operator==(const ConstResidueAPI& rhs) const {
    derived().check_invariants("operator==");
    return derived().res_ptr() == rhs.derived().res_ptr();
  }
};

template <typename Derived, typename RRef> class ResidueAPI : public ConstResidueAPI<Derived> {
private:
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:

  using ConstResidueAPI<Derived>::id;
  using ConstResidueAPI<Derived>::name;
  using ConstResidueAPI<Derived>::index;

  [[maybe_unused]] constexpr const RRef& id(const ResidueId& value) const& {
    derived().check_invariants("id");
    derived().res_ptr()->id = value;
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] constexpr RRef id(const ResidueId& value) const&& {
    derived().check_invariants("id");
    derived().res_ptr()->id = value;
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] constexpr const RRef& id(const ResidueIdSerial& value) const& {
    derived().check_invariants("id");
    derived().res_ptr()->id = value;
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] constexpr RRef id(const ResidueIdSerial& value) const&& {
    derived().check_invariants("id");
    derived().res_ptr()->id = value;
    return static_cast<const RRef&>(derived());
  }

  template <int N>[[maybe_unused]] constexpr const RRef& name(const char (&value)[N]) const& {
    derived().check_invariants("name");
    derived().res_ptr()->name = ResidueName(value);
    return static_cast<const RRef&>(derived());
  }

  template <int N>[[maybe_unused]] constexpr RRef name(const char (&value)[N]) const&& {
    derived().check_invariants("name");
    derived().res_ptr()->name = ResidueName(value);
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] const RRef& name(const std::string& value) const& {
    derived().check_invariants("name");
    derived().res_ptr()->name = ResidueName(value);
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] RRef name(const std::string& value) const&& {
    derived().check_invariants("name");
    derived().res_ptr()->name = ResidueName(value);
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] constexpr const RRef& name(const ResidueName& value) const& {
    derived().check_invariants("name");
    derived().res_ptr()->name = value;
    return static_cast<const RRef&>(derived());
  }

  [[maybe_unused]] constexpr RRef name(const ResidueName& value) const&& {
    derived().check_invariants("name");
    derived().res_ptr()->name = value;
    return static_cast<const RRef&>(derived());
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    derived().check_invariants("molecule");
    return MoleculeRef(*derived().res_ptr()->molecule);
  }

  [[nodiscard]] constexpr Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().res_ptr()->molecule->frame;
  }

  /// Atoms of the residue
  [[nodiscard]] AtomSpan atoms() const {
    derived().check_invariants("atoms");
    return AtomSpan{derived().res_ptr()->atoms};
  }

  /// Atom coordinates of the molecule
  [[nodiscard]] CoordSpan coords() const {
    derived().check_invariants("coords");
    return atoms().coords();
  }

  /// Next residue in the molecule
  [[nodiscard]] std::optional<RRef> next() const {
    if (derived().res_ptr() + 1 < derived().res_ptr()->molecule->residues.begin() + molecule().size()) {
      return RRef(*(derived().res_ptr() + 1));
    }
    return std::nullopt;
  }

  /// Previous residue in the molecule
  [[nodiscard]] std::optional<RRef> prev() const {
    if (derived().res_ptr() >= derived().res_ptr()->molecule->residues.begin() + 1) {
      return RRef(*(derived().res_ptr() - 1));
    }
    return std::nullopt;
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<AtomRef> operator[](const AtomName& name) const;
  [[nodiscard]] std::optional<AtomRef> operator[](const char* name) const;
  [[nodiscard]] std::optional<AtomRef> operator[](const std::string& name) const;

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  [[nodiscard]] AtomRef add_atom() const;

};

} // namespace xmol::proxy::references::api