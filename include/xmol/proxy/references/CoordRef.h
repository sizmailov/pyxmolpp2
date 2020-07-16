#pragma once

#include "ConstCoordRef.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {
/** @brief Lightweight XYZ reference
 * */
class CoordRef : public CoordSettersMixin<CoordRef> {
public:
  CoordRef(const CoordRef& rhs) = default;
  CoordRef(CoordRef&& rhs) noexcept = default;
  CoordRef& operator=(const CoordRef& rhs) = default;
  CoordRef& operator=(CoordRef&& rhs) noexcept = default;

  /// Check if references point to same data
  bool operator!=(const CoordRef& rhs) const {
    return m_cref.coord_ptr() != rhs.m_cref.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const CoordRef& rhs) const {
    return m_cref.coord_ptr() == rhs.m_cref.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  operator const XYZ&() const { return *m_cref.m_coord; }

protected:
  ConstCoordRef m_cref;

private:
  friend CoordGettersMixin<CoordRef>;
  friend CoordSettersMixin<CoordRef>;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSelection;
  friend CoordSelection;
  friend ProxySpan<CoordRef, XYZ>;
  friend ResidueSelection;
  friend Selection<CoordRef>;
  friend smart::CoordSmartSelection;
  explicit CoordRef(XYZ& coord);

  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_cref.m_coord; }
  [[nodiscard]] constexpr XYZ*& coord_ptr() { return m_cref.m_coord; }

  constexpr void check_invariants(const char*) const {};

  CoordRef(XYZ* ptr, XYZ*);
  void advance() { m_cref.advance(); }
  CoordRef() = default; // constructs object in invalid state (with nullptrs)
};

template <> struct Selection<proxy::CoordRef>::LessThanComparator {
  bool operator()(const proxy::CoordRef& p1, const proxy::CoordRef& p2) { return p1.coord_ptr() < p2.coord_ptr(); }
};

} // namespace xmol::proxy