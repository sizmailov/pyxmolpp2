#include "xmol/geometry/alignment.h"
#include "gsl/gsl_assert"
#include "xmol/geometry/exceptions.h"

#include <iostream>

using namespace xmol::geometry;

Transformation3d xmol::geometry::calc_alignment(
    const std::vector<xmol::geometry::XYZ>& reference,
    const std::vector<xmol::geometry::XYZ>& variable) {
  if (GSL_UNLIKELY(reference.size() != variable.size())) {
    throw AlignmentError("reference.size(" + std::to_string(reference.size()) +
                         ") != variable.size (=" +
                         std::to_string(variable.size()) + ")");
  }
  if (GSL_UNLIKELY(reference.size() < 3)) {
    throw AlignmentError("reference.size(" + std::to_string(reference.size()) +
                         ") < 3");
  }

  size_t N = reference.size();

  Eigen::MatrixXd X(N, 3);
  Eigen::MatrixXd Y(N, 3);

  auto xc = calc_geom_center(reference);
  auto yc = calc_geom_center(variable);

  for (size_t i = 0; i < N; i++) {
    X(i, 0) = reference[i].x() - xc.x();
    X(i, 1) = reference[i].y() - xc.y();
    X(i, 2) = reference[i].z() - xc.z();
  }

  for (size_t i = 0; i < N; i++) {
    Y(i, 0) = variable[i].x() - yc.x();
    Y(i, 1) = variable[i].y() - yc.y();
    Y(i, 2) = variable[i].z() - yc.z();
  }

  Eigen::Matrix3d C = X.transpose() * Y;

  Eigen::JacobiSVD<Eigen::Matrix3d> svd(C, Eigen::ComputeFullU |
                                               Eigen::ComputeFullV);

  Eigen::Matrix3d V = svd.matrixV();
  Eigen::Matrix3d W = svd.matrixU();
  Rotation3d R(W * V.transpose());
  if (R.get_underlying_matrix().determinant()< 0){
    Eigen::Matrix3d P;
    P << 1, 0, 0, 0, 1, 0, 0, 0, -1;
    R = Rotation3d(W * P * V.transpose());
  }

  Translation3d T(xc - R.transform(yc));
  return Transformation3d(R, T);
}

XYZ xmol::geometry::calc_geom_center(
    const std::vector<xmol::geometry::XYZ>& coordinates) {
  if (GSL_UNLIKELY(coordinates.empty())) {
    throw GeometryException("reference.size(" +
                            std::to_string(coordinates.size()) + ") == 0");
  }
  XYZ c;
  for (auto& x : coordinates) {
    c += x;
  }
  return c / coordinates.size();
}

double
xmol::geometry::calc_rmsd(const std::vector<xmol::geometry::XYZ>& reference,
                          const std::vector<xmol::geometry::XYZ>& variable) {
  if (GSL_UNLIKELY(reference.size() != variable.size())) {
    throw GeometryException(
        "reference.size(" + std::to_string(reference.size()) +
        ") != variable.size (=" + std::to_string(variable.size()) + ")");
  }
  if (GSL_UNLIKELY(reference.empty())) {
    throw GeometryException("reference.size(" +
                            std::to_string(reference.size()) + ") == 0");
  }
  double sum = 0;
  for (size_t i = 0; i < reference.size(); i++) {
    sum += (reference[i] - variable[i]).len2();
  }
  return std::sqrt(sum / reference.size());
}

double
xmol::geometry::calc_rmsd(const std::vector<xmol::geometry::XYZ>& reference,
                          const std::vector<xmol::geometry::XYZ>& variable,
                          const xmol::geometry::Transformation3d& alignment) {
  if (GSL_UNLIKELY(reference.size() != variable.size())) {
    throw GeometryException(
        "reference.size(" + std::to_string(reference.size()) +
        ") != variable.size (=" + std::to_string(variable.size()) + ")");
  }
  if (GSL_UNLIKELY(reference.empty())) {
    throw GeometryException("reference.size(" +
                            std::to_string(reference.size()) + ") == 0");
  }
  double sum = 0;
  for (size_t i = 0; i < reference.size(); i++) {
    sum += (reference[i] - alignment.transform(variable[i])).len2();
  }
  return std::sqrt(sum / reference.size());
}
