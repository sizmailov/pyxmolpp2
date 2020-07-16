#pragma once

#include "mixins/CoordRefMixin.h"

namespace xmol::proxy {

class CoordConstRef : public CoordGettersMixin<CoordConstRef> {
public:
  constexpr CoordConstRef(const CoordConstRef& rhs) = default;
  constexpr CoordConstRef(CoordConstRef&& rhs) noexcept = default;
  constexpr CoordConstRef& operator=(const CoordConstRef& rhs) = default;
  constexpr CoordConstRef& operator=(CoordConstRef&& rhs) noexcept = default;

  bool operator!=(const CoordConstRef& rhs) const {
    return coord_ptr() != rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const CoordConstRef& rhs) const {
    return coord_ptr() == rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

private:
  XYZ* m_coord = nullptr;

  constexpr XYZ* const coord_ptr() const { return m_coord; }
  constexpr XYZ* const& coord_ptr() { return m_coord; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() { ++m_coord; }

  constexpr CoordConstRef() = default; // constructs object in invalid state (with nullptrs)

  explicit CoordConstRef(XYZ& coord);
  CoordConstRef(XYZ* ptr, XYZ* end);

private:
  friend CoordGettersMixin<CoordConstRef>;
  friend CoordRef;
};
} // namespace xmol::proxy