#pragma once

#include "mixins/CoordRefMixin.h"

namespace xmol::proxy {

class ConstCoordRef : public CoordGettersMixin<ConstCoordRef> {
public:
  constexpr ConstCoordRef(const ConstCoordRef& rhs) = default;
  constexpr ConstCoordRef(ConstCoordRef&& rhs) noexcept = default;
  constexpr ConstCoordRef& operator=(const ConstCoordRef& rhs) = default;
  constexpr ConstCoordRef& operator=(ConstCoordRef&& rhs) noexcept = default;

  bool operator!=(const ConstCoordRef& rhs) const {
    return coord_ptr() != rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const ConstCoordRef& rhs) const {
    return coord_ptr() == rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

private:
  XYZ* m_coord = nullptr;

  constexpr XYZ* const coord_ptr() const { return m_coord; }
  constexpr XYZ* const& coord_ptr() { return m_coord; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() { ++m_coord; }

  constexpr ConstCoordRef() = default; // constructs object in invalid state (with nullptrs)

  explicit ConstCoordRef(XYZ& coord);
  ConstCoordRef(XYZ* ptr, XYZ* end);

private:
  friend CoordGettersMixin<ConstCoordRef>;
  friend CoordRef;
};
} // namespace xmol::proxy