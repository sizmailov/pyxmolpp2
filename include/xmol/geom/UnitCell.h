#pragma once

#include <optional>

#include "AngleValue.h"
#include "XYZ.h"

namespace xmol::geom {

class UnitCell {
public:
  struct ClosestImage {
    std::tuple<int, int, int> shift_int; /// shift as (i,j,k)
    XYZ shift;                           /// shift as XYZ, equal to  translation_vector(*shift_int)
    XYZ pos;                             /// closest image position
    double distance;                     /// distance to closest position
  };

  UnitCell(const XYZ& v1, const XYZ& v2, const XYZ& v3) : v{{v1, v2, v3}} {}
  UnitCell(double a, double b, double c, AngleValue alpha, AngleValue beta, AngleValue gamma) {
    using XYZ = XYZ;

    auto& v1 = v[0];
    auto& v2 = v[1];
    auto& v3 = v[2];

    v1 = XYZ(a, 0.0, 0.0);
    v2 = XYZ(b * cos(gamma), b * sin(gamma), 0);

    v3.set_x(c * cos(beta));
    v3.set_y(b / v2.y() * c * cos(alpha) - v2.x() / v2.y() * v3.x());
    v3.set_z(sqrt(c * c - v3.x() * v3.x() - v3.y() * v3.y()));
  }

  XYZ translation_vector(int i, int j, int k) const { return i * v[0] + j * v[1] + k * v[2]; }

  void scale_by(double factor) {
    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;
    if (A_inv) {
      *A_inv /= factor * factor;
    }
  }

  const XYZ& operator[](int i) const { return v[i]; }

  AngleValue alpha() { return v[1].angle(v[2]); }

  AngleValue beta() { return v[0].angle(v[2]); }

  AngleValue gamma() { return v[0].angle(v[1]); }

  double a() { return v[0].len(); }
  double b() { return v[1].len(); }
  double c() { return v[2].len(); }

  double volume() const { return ::fabs(v[0].cross(v[1]).dot(v[2])); }

  void scale_to_volume(double vol) { scale_by(::cbrt(vol / volume())); }

  [[nodiscard]] ClosestImage closest_image_to(const XYZ& ref, const XYZ& var) const;
  [[nodiscard]] static UnitCell unit_cubic_cell(); // Returns cubic cell of volume 1

private:
  std::array<XYZ, 3> v;
  mutable std::optional<Eigen::Matrix3d> A_inv;

  void update_matrix() const {
    Eigen::Matrix3d A;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        A(i, j) = v[i].dot(v[j]);
      }
    }
    A_inv = A.inverse();
  }
};

} // namespace xmol::geom