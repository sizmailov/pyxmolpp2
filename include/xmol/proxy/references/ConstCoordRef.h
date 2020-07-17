#pragma once

#include "xmol/base.h"
#include "xmol/geom/AngleValue.h"
#include "xmol/geom/XYZ.h"

namespace xmol::proxy {

class ConstCoordRef {
public:
  constexpr ConstCoordRef(const ConstCoordRef& rhs) = default;
  constexpr ConstCoordRef(ConstCoordRef&& rhs) noexcept = default;
  constexpr ConstCoordRef& operator=(const ConstCoordRef& rhs) = default;
  constexpr ConstCoordRef& operator=(ConstCoordRef&& rhs) noexcept = default;

  [[nodiscard]] double x() const {
    check_invariants("x");
    return coord_ptr()->x();
  }

  [[nodiscard]] double y() const {
    check_invariants("y");
    return coord_ptr()->y();
  }

  [[nodiscard]] double z() const {
    check_invariants("z");
    return coord_ptr()->z();
  }

  [[nodiscard]] double len2() const {
    check_invariants("len2");
    return coord_ptr()->len2();
  }

  [[nodiscard]] double len() const {
    check_invariants("len");
    return coord_ptr()->len();
  }

  [[nodiscard]] const CoordEigenVector& _eigen() const {
    check_invariants("_eigen");
    return coord_ptr()->_eigen();
  }

  [[nodiscard]] XYZ cross(const XYZ& other) const {
    check_invariants("cross");
    return coord_ptr()->cross(other);
  }

  [[nodiscard]] double dot(const XYZ& other) const {
    check_invariants("cross");
    return coord_ptr()->dot(other);
  }

  [[nodiscard]] double distance2(const XYZ& other) const {
    check_invariants("distance2");
    return coord_ptr()->distance2(other);
  }

  [[nodiscard]] double distance(const XYZ& other) const {
    check_invariants("distance");
    return coord_ptr()->distance(other);
  }

  [[nodiscard]] geom::AngleValue angle(const XYZ& other) const {
    check_invariants("angle");
    return coord_ptr()->angle(other);
  }

  [[nodiscard]] geom::AngleValue angle_between(const XYZ& a, const XYZ& c) const {
    check_invariants("angle_between");
    return coord_ptr()->angle_between(a, c);
  }

  // TODO: dihedral

  bool operator!=(const ConstCoordRef& rhs) const {
    return coord_ptr() != rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const ConstCoordRef& rhs) const {
    return coord_ptr() == rhs.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  explicit operator const XYZ&() const& { return *m_coord; }
  explicit operator XYZ() const&& { return *m_coord; }

private:
  XYZ* m_coord = nullptr;

  [[nodiscard]] constexpr const XYZ* coord_ptr() const { return m_coord; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() { ++m_coord; }
  void rebase(XYZ* from, XYZ* to) { m_coord = to + (m_coord - from); }

  constexpr ConstCoordRef() = default; // constructs object in invalid state (with nullptrs)

  explicit ConstCoordRef(XYZ& coord);
  ConstCoordRef(XYZ* ptr, XYZ* end);

private:
  friend CoordRef;
};
} // namespace xmol::proxy