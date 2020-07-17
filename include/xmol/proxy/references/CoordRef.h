#pragma once

#include "ConstCoordRef.h"
#include "xmol/proxy/ProxySpan.h"
#include "xmol/proxy/Selection.h"

namespace xmol::proxy {
/** @brief Lightweight XYZ reference
 * */
class CoordRef {
public:
  CoordRef(const CoordRef& rhs) = default;
  CoordRef(CoordRef&& rhs) noexcept = default;
  CoordRef& operator=(const CoordRef& rhs) = default;
  CoordRef& operator=(CoordRef&& rhs) noexcept = default;

  [[nodiscard]] double x() const {
    check_invariants("x");
    return coord_ptr()->x();
  }

  [[maybe_unused]] const CoordRef& x(const double& value) const& {
    check_invariants("x");
    coord_ptr()->set_x(value);
    return *this;
  }

  [[maybe_unused]] CoordRef x(const float& value) const&& {
    check_invariants("x");
    coord_ptr()->set_x(value);
    return *this;
  }

  [[nodiscard]] double y() const {
    check_invariants("y");
    return coord_ptr()->y();
  }

  [[maybe_unused]] const CoordRef& y(const double& value) const& {
    check_invariants("y");
    coord_ptr()->set_y(value);
    return *this;
  }

  [[maybe_unused]] CoordRef y(const float& value) const&& {
    check_invariants("y");
    coord_ptr()->set_y(value);
    return *this;
  }

  [[nodiscard]] double z() const {
    check_invariants("z");
    return coord_ptr()->z();
  }

  [[maybe_unused]] const CoordRef& z(const double& value) const& {
    check_invariants("z");
    coord_ptr()->set_z(value);
    return *this;
  }

  [[maybe_unused]] CoordRef z(const float& value) const&& {
    check_invariants("z");
    coord_ptr()->set_z(value);
    return *this;
  }

  [[maybe_unused]] const CoordRef& set(const XYZ& value) const& {
    check_invariants("set");
    *coord_ptr() = value;
    return *this;
  }

  [[maybe_unused]] CoordRef set(const XYZ& value) const&& {
    check_invariants("set");
    *coord_ptr() = value;
    return *this;
  }

  [[nodiscard]] double len2() const {
    check_invariants("len2");
    return coord_ptr()->len2();
  }

  [[nodiscard]] double len() const {
    check_invariants("len");
    return coord_ptr()->len();
  }

  [[nodiscard]] CoordEigenVector& _eigen() const {
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

  /// Check if references point to same data
  bool operator!=(const CoordRef& rhs) const {
    return m_cref.coord_ptr() !=
           rhs.m_cref.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const CoordRef& rhs) const {
    return m_cref.coord_ptr() ==
           rhs.m_cref.coord_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  [[maybe_unused]] const CoordRef& operator+=(const XYZ& value) const & {
    check_invariants("operator+=");
    coord_ptr()->operator+=(value);
    return *this;
  }

  [[maybe_unused]] const CoordRef& operator-=(const XYZ& value) const & {
    check_invariants("operator-=");
    coord_ptr()->operator-=(value);
    return *this;
  }

  [[maybe_unused]] const CoordRef& operator/=(const double& value) const & {
    check_invariants("operator/=");
    coord_ptr()->operator/=(value);
    return *this;
  }

  [[maybe_unused]] const CoordRef& operator*=(const double& value) const & {
    check_invariants("operator*=");
    coord_ptr()->operator*=(value);
    return *this;
  }

  operator XYZ&() const& { return *m_cref.m_coord; }
  operator XYZ() const&& { return *m_cref.m_coord; }

protected:
  ConstCoordRef m_cref;

private:
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

  constexpr void check_invariants(const char*) const {};

  CoordRef(XYZ* ptr, XYZ*);
  void advance() { m_cref.advance(); }
  void rebase(XYZ* from, XYZ* to) { m_cref.rebase(from, to); }
  CoordRef() = default; // constructs object in invalid state (with nullptrs)
};

template <> struct Selection<proxy::CoordRef>::LessThanComparator {
  bool operator()(const proxy::CoordRef& p1, const proxy::CoordRef& p2) { return p1.coord_ptr() < p2.coord_ptr(); }
};

} // namespace xmol::proxy