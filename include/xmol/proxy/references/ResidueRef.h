#pragma once

#include "ConstResidueRef.h"
#include "api/ResidueAPI.h"

namespace xmol::proxy {

/** @brief Lightweight residue reference
 *
 * For ref-counting reference see @ref smart::ResidueSmartRef
 * */
class ResidueRef : public api::ResidueAPI<ResidueRef> {
public:
  constexpr ResidueRef(const ResidueRef& rhs) = default;
  constexpr ResidueRef(ResidueRef&& rhs) noexcept = default;
  constexpr ResidueRef& operator=(const ResidueRef& rhs) = default;
  constexpr ResidueRef& operator=(ResidueRef&& rhs) noexcept = default;

  /// Create smart reference from this
  [[nodiscard]] ResidueSmartRef smart() const;

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

  template<typename, typename>
  friend class api::ResidueAPI;

  template<typename, typename>
  friend class api::ConstResidueAPI;

  template<typename, typename>
  friend class api::MoleculeAPI;

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