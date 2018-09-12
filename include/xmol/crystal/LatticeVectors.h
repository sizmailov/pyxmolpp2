#pragma once

#include "xmol/geometry/AngleValue.h"
#include "xmol/geometry/XYZ.h"

namespace xmol {
namespace crystal {

class LatticeVectors {
public:
  LatticeVectors(const geometry::XYZ& v1, const geometry::XYZ& v2, const geometry::XYZ& v3) : v{{v1, v2, v3}} {}
  LatticeVectors(double a, double b, double c, geometry::AngleValue alpha, geometry::AngleValue beta,
                 geometry::AngleValue gamma) {
    using XYZ = geometry::XYZ;

    auto& v1 = v[0];
    auto& v2 = v[1];
    auto& v3 = v[2];

    v1 = XYZ(a, 0.0, 0.0);
    v2 = XYZ(b * cos(gamma), b * sin(gamma), 0);

    v3.set_x(c * cos(beta));
    v3.set_y(b / v2.y() * c * cos(alpha) - v2.x() / v2.y() * v3.x());
    v3.set_z(sqrt(c * c - v3.x() * v3.x() - v3.y() * v3.y()));
  }

  inline geometry::XYZ translate(const geometry::XYZ& r, int i, int j, int k) const { return r + get_shift(i, j, k); }

  inline geometry::XYZ get_shift(int i, int j, int k) const { return i * v[0] + j * v[1] + k * v[2]; }

  void scale_by(double factor) {
    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;
  }

  const geometry::XYZ& operator[](int i) const { return v[i]; }

  geometry::XYZ& operator[](int i) { return v[i]; }
private:
  std::array<geometry::XYZ, 3> v;
};

struct BestShiftFinder {
  explicit BestShiftFinder(const LatticeVectors& latticeVectors) : v(latticeVectors) {
    Eigen::Matrix3d A;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        A(i, j) = v[i].dot(v[j]);
      }
    }
    A_inv = A.inverse();
  };
  void scale_lattice_by(double factor) {
    A_inv /= factor*factor;
    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;
  }
  std::pair<double, geometry::XYZ> find_best_shift(const geometry::XYZ& ref, const geometry::XYZ& var) const {
    auto delta = ref - var;
    Eigen::Vector3d approx = A_inv * Eigen::Vector3d(delta.dot(v[0]), delta.dot(v[1]), delta.dot(v[2]));
    const int i = std::round(approx[0]);
    const int j = std::round(approx[1]);
    const int k = std::round(approx[2]);
    geometry::XYZ best_shift(i,j,k);
    double best_length2 = (ref - (var+best_shift)).len2();
    for (int di = -1; di < 2; di++)
      for (int dj = -1; dj < 2; dj++)
        for (int dk = -1; dk < 2; dk++) {
          auto shift = v.get_shift(i + di, j + dj, k + dk);
          auto length2 = (ref - (var + shift)).len2();
          if (length2 < best_length2) {
            best_shift = shift;
            best_length2 = length2;
          }
        }
    return std::make_pair(std::sqrt(best_length2), best_shift);
  }

private:
  Eigen::Matrix3d A_inv;
  LatticeVectors v;
};
}
}