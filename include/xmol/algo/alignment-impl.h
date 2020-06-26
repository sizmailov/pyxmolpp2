#pragma once
#include "alignment.h"
#include "xmol/base.h"

namespace xmol::algo {

/// Calculate rotational alignment on precentered coordinate matrices
///
/// @tparam MatrixA reference coordinates, Eigen [N*3] matrix or equivalent expression
/// @tparam MatrixB coordinates to align, Eigen [N*3] matrix or equivalent expression
template <typename MatrixA, typename MatrixB>
geom::affine::Rotation3d calc_alignment_precentered_impl(const MatrixA& X, const MatrixB& Y) {
  if (X.rows() != Y.rows()) {
    throw geom::GeomError("alignment: reference.size (=" + std::to_string(X.rows()) +
                          ") != variable.size (=" + std::to_string(Y.rows()) + ")");
  }
  if (X.rows() < 3) {
    throw geom::GeomError("alignment: reference.size (=" + std::to_string(X.rows()) + ") < 3");
  }

  Eigen::Matrix3d C = X.transpose() * Y;

  Eigen::JacobiSVD<Eigen::Matrix3d> svd(C, Eigen::ComputeFullU | Eigen::ComputeFullV);

  const Eigen::Matrix3d& V = svd.matrixV();
  const Eigen::Matrix3d& W = svd.matrixU();
  geom::affine::Rotation3d R(W * V.transpose());

  if (R.get_underlying_matrix().determinant() < 0) {
    Eigen::Matrix3d P;
    P << 1, 0, 0, 0, 1, 0, 0, 0, -1;
    R = geom::affine::Rotation3d(W * P * V.transpose());
  }

  return R;
}

/// Calculate alignment of coordinate matrices
///
/// @tparam MatrixA reference coordinates, Eigen [N*3] matrix or equivalent expression
/// @tparam MatrixB coordinates to align, Eigen [N*3] matrix or equivalent expression
template <typename MatrixA, typename MatrixB>
geom::affine::Transformation3d calc_alignment_impl(const MatrixA& X, const MatrixB& Y) {
  CoordEigenVector xc = X.colwise().mean();
  CoordEigenVector yc = Y.colwise().mean();

  auto R = calc_alignment_precentered_impl(X.rowwise() - xc, Y.rowwise() - yc);
  auto T = geom::affine::Translation3d(XYZ(xc) - R.transform(XYZ(yc)));

  return geom::affine::Transformation3d(R, T);
}

template <typename MatrixA, typename MatrixB, typename MassVector>
geom::affine::Transformation3d calc_alignment_weighted_impl(const MatrixA& X, const MatrixB& Y,
                                                            const MassVector& weight) {
  double total_mass = weight.sum();

  if (total_mass < 1e-3) {
    throw geom::GeomError("Total weight is too low, check you inputs");
  }

  CoordEigenMatrix X_scaled = X.array().colwise() * weight.array() / total_mass;
  CoordEigenMatrix Y_scaled = Y.array().colwise() * weight.array() / total_mass;

  CoordEigenVector xc = X_scaled.colwise().sum();
  CoordEigenVector yc = Y_scaled.colwise().sum();

  auto R = calc_alignment_precentered_impl(X.rowwise() - xc, Y.rowwise() - yc);
  auto T = geom::affine::Translation3d(XYZ(xc) - R.transform(XYZ(yc)));

  return geom::affine::Transformation3d(R, T);
}

template <typename MatrixA, typename MatrixB> double calc_rmsd_impl(const MatrixA& reference, const MatrixB& variable) {
  return std::sqrt((reference - variable).array().square().sum() / reference.rows());
}

template <typename MatrixA> Eigen::Matrix3d calc_inertia_tensor_impl(const MatrixA& coords) {
  assert(coords.cols() == 3);
  CoordEigenVector center(coords.colwise().mean());
  const int X = 0, Y = 1, Z = 2;
  double x2 = (coords.col(X).array() - center(X)).square().sum();
  double y2 = (coords.col(Y).array() - center(Y)).square().sum();
  double z2 = (coords.col(Z).array() - center(Z)).square().sum();
  double xx = y2 + z2;
  double yy = x2 + z2;
  double zz = y2 + x2;
  double xy = -((coords.col(X).array() - center(X)) * (coords.col(Y).array() - center(Y))).sum();
  double xz = -((coords.col(X).array() - center(X)) * (coords.col(Z).array() - center(Z))).sum();
  double yz = -((coords.col(Y).array() - center(Y)) * (coords.col(Z).array() - center(Z))).sum();

  Eigen::Matrix3d result;
  result << xx, xy, xz, xy, yy, yz, xz, yz, zz;
  return result;
}

template <typename CoordMatrix, typename MassMatrix>
Eigen::Matrix3d calc_inertia_tensor_impl(const CoordMatrix& coords, const MassMatrix& mass) {
  assert(coords.cols() == 3);
  CoordEigenVector center((coords.array().colwise() * mass.array()).colwise().mean() / mass.sum());
  const int X = 0, Y = 1, Z = 2;
  double x2 = ((coords.col(X).array().square() - center(X)) * mass.array()).sum();
  double y2 = ((coords.col(Y).array().square() - center(Y)) * mass.array()).sum();
  double z2 = ((coords.col(Z).array().square() - center(Z)) * mass.array()).sum();
  double xx = y2 + z2;
  double yy = x2 + z2;
  double zz = y2 + x2;
  double xy =
      -(((coords.col(X).array() - center(X)) * (coords.col(Y).array() - center(Y))).array() * mass.array()).sum();
  double xz =
      -(((coords.col(X).array() - center(X)) * (coords.col(Z).array() - center(Z))).array() * mass.array()).sum();
  double yz =
      -(((coords.col(Y).array() - center(Y)) * (coords.col(Z).array() - center(Z))).array() * mass.array()).sum();

  Eigen::Matrix3d result;
  result << xx, xy, xz, xy, yy, yz, xz, yz, zz;
  return result;
}

} // namespace xmol::algo