#pragma once

#include "alignment.h"


namespace xmol::v1::geom {

template <typename MatrixA, typename MatrixB>
affine::Transformation3d calc_alignment_impl(const MatrixA& X, const MatrixB& Y) {
  if (X.rows() != Y.rows()) {
    throw GeomError("reference.size(" + std::to_string(X.size()) + ") != variable.size (=" + std::to_string(Y.size()) +
                    ")");
  }
  if (X.rows() < 3) {
    throw GeomError("reference.size(" + std::to_string(X.size()) + ") < 3");
  }

  CoordEigenVector xc = X.rowwise().mean();
  CoordEigenVector yc = Y.rowwise().mean();

  Eigen::Matrix3d C = (X.colwise() - xc) * (Y.colwise() - yc).transpose();

  Eigen::JacobiSVD<Eigen::Matrix3d> svd(C, Eigen::ComputeFullU | Eigen::ComputeFullV);

  const Eigen::Matrix3d& V = svd.matrixV();
  const Eigen::Matrix3d& W = svd.matrixU();
  affine::Rotation3d R(W * V.transpose());
  if (R.get_underlying_matrix().determinant() < 0) {
    Eigen::Matrix3d P;
    P << 1, 0, 0, 0, 1, 0, 0, 0, -1;
    R = affine::Rotation3d(W * P * V.transpose());
  }

  affine::Translation3d T(XYZ(xc) - R.transform(XYZ(yc)));
  return affine::Transformation3d(R, T);
}

template <typename MatrixA, typename MatrixB> double calc_rmsd_impl(const MatrixA& reference, const MatrixB& variable) {
  return (reference - variable).array().square().rowwise().sum().sqrt().mean();
}

} // namespace xmol::v1::geom