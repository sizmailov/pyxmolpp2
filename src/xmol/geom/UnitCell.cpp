#include "xmol/geom/UnitCell.h"

using namespace xmol::geom;

UnitCell::ClosestImage UnitCell::closest_image_to(const XYZ& ref, const XYZ& var) const {
  if (!A_inv) {
    update_matrix();
  }
  auto delta = ref - var;
  Eigen::Vector3d approx = *A_inv * Eigen::Vector3d(delta.dot(v[0]), delta.dot(v[1]), delta.dot(v[2]));
  const int i = std::round(approx[0]);
  const int j = std::round(approx[1]);
  const int k = std::round(approx[2]);
  ClosestImage result{
      std::make_tuple(i, j, k),
      translation_vector(i, j, k),
  };
  double distance2 = (ref - (var + result.shift)).len2(); // temporary store squared value
  for (int di = -1; di < 2; di++)
    for (int dj = -1; dj < 2; dj++)
      for (int dk = -1; dk < 2; dk++) {
        auto shift = translation_vector(i + di, j + dj, k + dk);
        auto length2 = (ref - (var + shift)).len2();
        if (length2 < result.distance) {
          result.shift = shift;
          result.shift_int = std::make_tuple(i + di, j + dj, k + dk);
          distance2 = length2; // temporary store squared value
        }
      }
  result.distance = std::sqrt(distance2);
  result.pos = var + result.shift;
  return result;
}

UnitCell UnitCell::unit_cubic_cell() { return UnitCell(geom::XYZ(1, 0, 0), geom::XYZ(0, 1, 0), geom::XYZ(0, 0, 1)); }
