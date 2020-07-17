#pragma once

#include "ConstResidueRef.h"

namespace xmol::proxy {

/** @brief Lightweight residue reference
 *
 * For ref-counting reference see @ref smart::ResidueSmartRef
 * */
class ResidueRef {
public:
  constexpr ResidueRef(const ResidueRef& rhs) = default;
  constexpr ResidueRef(ResidueRef&& rhs) noexcept = default;
  constexpr ResidueRef& operator=(const ResidueRef& rhs) = default;
  constexpr ResidueRef& operator=(ResidueRef&& rhs) noexcept = default;

  [[nodiscard]] constexpr ResidueId id() const {
    check_invariants("id");
    return res_ptr()->id;
  }

  [[maybe_unused]] constexpr const ResidueRef& id(const ResidueId& value) const& {
    check_invariants("id");
    res_ptr()->id = value;
    return *this;
  }

  [[maybe_unused]] constexpr ResidueRef id(const ResidueId& value) const&& {
    check_invariants("id");
    res_ptr()->id = value;
    return *this;
  }

  [[maybe_unused]] constexpr const ResidueRef& id(const ResidueIdSerial& value) const& {
    check_invariants("id");
    res_ptr()->id = value;
    return *this;
  }

  [[maybe_unused]] constexpr ResidueRef id(const ResidueIdSerial& value) const&& {
    check_invariants("id");
    res_ptr()->id = value;
    return *this;
  }

  [[nodiscard]] constexpr ResidueName name() const {
    check_invariants("name");
    return res_ptr()->name;
  }

  template <int N>[[maybe_unused]] constexpr const ResidueRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    res_ptr()->name = ResidueName(value);
    return *this;
  }

  template <int N>[[maybe_unused]] constexpr ResidueRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    res_ptr()->name = ResidueName(value);
    return *this;
  }

  [[maybe_unused]] const ResidueRef& name(const std::string& value) const& {
    check_invariants("name");
    res_ptr()->name = ResidueName(value);
    return *this;
  }

  [[maybe_unused]] const ResidueRef name(const std::string& value) const&& {
    check_invariants("name");
    res_ptr()->name = ResidueName(value);
    return *this;
  }

  [[maybe_unused]] constexpr const ResidueRef& name(const ResidueName& value) const& {
    check_invariants("name");
    res_ptr()->name = value;
    return *this;
  }

  [[maybe_unused]] constexpr ResidueRef name(const ResidueName& value) const&& {
    check_invariants("name");
    res_ptr()->name = value;
    return *this;
  }

  [[nodiscard]] ResidueIndex index() const;

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return res_ptr()->atoms.m_begin == res_ptr()->atoms.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return res_ptr()->atoms.m_end - res_ptr()->atoms.m_begin;
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    check_invariants("molecule");
    return MoleculeRef(*res_ptr()->molecule);
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return *res_ptr()->molecule->frame;
  }

  /// Atoms of the residue
  [[nodiscard]] AtomSpan atoms() const { return AtomSpan{res_ptr()->atoms}; }

  /// Atom coordinates of the molecule
  [[nodiscard]] CoordSpan coords() const { return atoms().coords(); }

  /// Next residue in the molecule
  [[nodiscard]] std::optional<ResidueRef> next() const {
    if (res_ptr() + 1 < res_ptr()->molecule->residues.begin() + molecule().size()) {
      return ResidueRef(*(c_ref.m_residue + 1));
    }
    return std::nullopt;
  }

  /// Previous residue in the molecule
  [[nodiscard]] std::optional<ResidueRef> prev() const {
    if (res_ptr() >= res_ptr()->molecule->residues.begin() + 1) {
      return ResidueRef(*(c_ref.m_residue - 1));
    }
    return std::nullopt;
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<AtomRef> operator[](const AtomName& name) const;
  [[nodiscard]] std::optional<AtomRef> operator[](const char* name) const;
  [[nodiscard]] std::optional<AtomRef> operator[](const std::string& name) const;

  /// Create smart reference from this
  [[nodiscard]] ResidueSmartRef smart() const;

  /// Check if references point to same data
  [[nodiscard]] constexpr bool operator!=(const ResidueRef& rhs) const {
    check_invariants("operator!=");
    return res_ptr() != rhs.res_ptr();
  }
  [[nodiscard]] constexpr bool operator==(const ResidueRef& rhs) const {
    check_invariants("operator==");
    return res_ptr() == rhs.res_ptr();
  }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  [[nodiscard]] AtomRef add_atom() const;

private:
  ConstResidueRef c_ref;

  template<typename, typename>
  friend class proxy::api::AtomAPI;

  friend AtomSelection;
  friend Frame;
  friend MoleculeRef;
  friend MoleculeSelection;
  friend ProxySpan<ResidueRef, BaseResidue>;
  friend ResidueSpan;
  friend ResidueSelection;
  friend Selection<ResidueRef>::LessThanComparator;

  friend ResidueSmartRef;
  friend smart::ResidueSmartSelection;

  constexpr void check_invariants(const char*) const {};

  [[nodiscard]] constexpr BaseResidue* res_ptr() const { return c_ref.m_residue; }

  constexpr explicit ResidueRef(BaseResidue& residue) : c_ref(residue){};

  constexpr ResidueRef(BaseResidue* ptr, BaseResidue*) : c_ref(*ptr){};
  constexpr void advance() { c_ref.advance(); }
  void rebase(BaseResidue* from, BaseResidue* to) { c_ref.rebase(from, to); }
  constexpr ResidueRef() = default; /// constructs object in invalid state (with nullptrs)
};

std::string to_string(const ResidueRef& res);

template <> struct Selection<proxy::ResidueRef>::LessThanComparator {
  bool operator()(const proxy::ResidueRef& p1, const proxy::ResidueRef& p2) { return p1.res_ptr() < p2.res_ptr(); }
};

} // namespace xmol::proxy