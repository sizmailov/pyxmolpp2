#pragma once
#include "alignment.h"
#include "xmol/base.h"

namespace xmol::algo {

template <typename MatrixA, typename MatrixB>
geom::affine::Transformation3d calc_alignment_impl(const MatrixA& X, const MatrixB& Y) {
  if (X.rows() != Y.rows()) {
    throw geom::GeomError("reference.size(" + std::to_string(X.size()) + ") != variable.size (=" + std::to_string(Y.size()) +
                    ")");
  }
  if (X.rows() < 3) {
    throw geom::GeomError("reference.size(" + std::to_string(X.size()) + ") < 3");
  }

  CoordEigenVector xc = X.rowwise().mean();
  CoordEigenVector yc = Y.rowwise().mean();

  Eigen::Matrix3d C = (X.colwise() - xc) * (Y.colwise() - yc).transpose();

  Eigen::JacobiSVD<Eigen::Matrix3d> svd(C, Eigen::ComputeFullU | Eigen::ComputeFullV);

  const Eigen::Matrix3d& V = svd.matrixV();
  const Eigen::Matrix3d& W = svd.matrixU();
  geom::affine::Rotation3d R(W * V.transpose());
  if (R.get_underlying_matrix().determinant() < 0) {
    Eigen::Matrix3d P;
    P << 1, 0, 0, 0, 1, 0, 0, 0, -1;
    R = geom::affine::Rotation3d(W * P * V.transpose());
  }

  geom::affine::Translation3d T(XYZ(xc) - R.transform(XYZ(yc)));
  return geom::affine::Transformation3d(R, T);
}

template <typename MatrixA, typename MatrixB> double calc_rmsd_impl(const MatrixA& reference, const MatrixB& variable) {
  return std::sqrt((reference - variable).array().square().sum() / reference.cols() );
}

template <typename MatrixA> Eigen::Matrix3d calc_inertia_tensor_impl(const MatrixA& coords) {
  CoordEigenVector center(coords.rowwise().mean());
  const int X=0,Y=1,Z=2;
  double x2 = coords.row(X).array().square().sum();
  double y2 = coords.row(Y).array().square().sum();
  double z2 = coords.row(Z).array().square().sum();
  double xx = y2 + z2;
  double yy = x2+z2;
  double zz = y2+x2;
  double xy = - ((coords.row(X).array() - center(X))*(coords.row(Y).array() - center(Y))).sum();
  double xz = - ((coords.row(X).array() - center(X))*(coords.row(Z).array() - center(Z))).sum();
  double yz = - ((coords.row(Y).array() - center(Y))*(coords.row(Z).array() - center(Z))).sum();

  Eigen::Matrix3d result;
  result << xx, xy, xz, xy, yy, yz, xz, yz, zz;
  return result;
}

template <typename CoordMatrix, typename MassMatrix> Eigen::Matrix3d calc_inertia_tensor_impl(const CoordMatrix& coords, const MassMatrix& mass) {
  CoordEigenVector center(coords.rowwise().mean());
  const int X=0,Y=1,Z=2;
  double x2 = (coords.row(X).array().square().array() * mass.array()).sum();
  double y2 = (coords.row(Y).array().square().array() * mass.array()).sum();
  double z2 = (coords.row(Z).array().square().array() * mass.array()).sum();
  double xx = y2 + z2;
  double yy = x2 + z2;
  double zz = y2 + x2;
  double xy = - (((coords.row(X).array() - center(X))*(coords.row(Y).array() - center(Y))).array() * mass.array()).sum();
  double xz = - (((coords.row(X).array() - center(X))*(coords.row(Z).array() - center(Z))).array() * mass.array()).sum();
  double yz = - (((coords.row(Y).array() - center(Y))*(coords.row(Z).array() - center(Z))).array() * mass.array()).sum();

  Eigen::Matrix3d result;
  result << xx, xy, xz, xy, yy, yz, xz, yz, zz;
  return result;
}

} // namespace xmol::geom