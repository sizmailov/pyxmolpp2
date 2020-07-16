#pragma once

#include "ConstResidueRef.h"

namespace xmol::proxy {

/** @brief Lightweight residue reference
 *
 * For ref-counting reference see @ref smart::ResidueSmartRef
 * */
class ResidueRef : public ResidueSettersMixin<ResidueRef> {
public:
  constexpr ResidueRef(const ResidueRef& rhs) = default;
  constexpr ResidueRef(ResidueRef&& rhs) noexcept = default;
  constexpr ResidueRef& operator=(const ResidueRef& rhs) = default;
  constexpr ResidueRef& operator=(ResidueRef&& rhs) noexcept = default;

  /// Index of the residue in frame (0-based)
  [[nodiscard]] ResidueIndex index() const noexcept;

  /// Atoms of the residue
  AtomSpan atoms() { return AtomSpan{res_ptr()->atoms}; }

  /// Atom coordinates of the molecule
  CoordSpan coords() { return atoms().coords(); }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const AtomName& name);
  std::optional<proxy::AtomRef> operator[](const char* name);
  std::optional<proxy::AtomRef> operator[](const std::string& name);

  /// Create smart reference from this
  smart::ResidueSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const { return res_ptr() != rhs.res_ptr(); }
  bool operator==(const ResidueRef& rhs) const { return res_ptr() == rhs.res_ptr(); }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  AtomRef add_atom();

private:
  ConstResidueRef c_ref;

  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeRef;
  friend MoleculeSelection;
  friend ProxySpan<ResidueRef, BaseResidue>;
  friend ResidueSpan;
  friend ResidueSelection;
  friend Selection<ResidueRef>::LessThanComparator;

  friend AtomGettersMixin<AtomRef>;
  friend AtomGettersMixin<smart::AtomSmartRef>;

  friend ResidueGettersMixin<ResidueRef>;
  friend ResidueSettersMixin<ResidueRef>;
  friend smart::ResidueSmartRef;
  friend smart::ResidueSmartSelection;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseResidue* res_ptr() const { return c_ref.m_residue; }
  constexpr BaseResidue*& res_ptr() { return c_ref.m_residue; }

  constexpr explicit ResidueRef(BaseResidue& residue) : c_ref(residue){};

  constexpr ResidueRef(BaseResidue* ptr, BaseResidue*) : c_ref(*ptr){};
  constexpr void advance() { c_ref.advance(); }
  constexpr ResidueRef() = default; /// constructs object in invalid state (with nullptrs)
};

std::string to_string(const ResidueRef& res);

template <> struct Selection<proxy::ResidueRef>::LessThanComparator {
  bool operator()(const proxy::ResidueRef& p1, const proxy::ResidueRef& p2) { return p1.res_ptr() < p2.res_ptr(); }
};

} // namespace xmol::proxy