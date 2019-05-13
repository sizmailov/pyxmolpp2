#include "xmol/geometry/Transformation3d.h"
#include "xmol/geometry/exceptions.h"
#include "gsl/gsl_assert"
#include <iostream>
using namespace xmol::geometry;

Rotation3d::Rotation3d() { m << 1, 0, 0, 0, 1, 0, 0, 0, 1; }

namespace {

inline double to_domain_11(double value) {
  return std::max(-1.0, std::min(1.0, value));
}
}

Rotation3d::Rotation3d(XYZ u, AngleValue theta) {
  u /= u.len();

  double cos_theta = cos(theta);
  double sin_theta = sin(theta);
  double cos_1 = 1 - cos_theta;

  m(0, 0) = u.x() * u.x() * cos_1 + cos_theta;
  m(0, 1) = u.x() * u.y() * cos_1 - u.z() * sin_theta;
  m(0, 2) = u.x() * u.z() * cos_1 + u.y() * sin_theta;

  m(1, 0) = u.y() * u.x() * cos_1 + u.z() * sin_theta;
  m(1, 1) = u.y() * u.y() * cos_1 + cos_theta;
  m(1, 2) = u.y() * u.z() * cos_1 - u.x() * sin_theta;

  m(2, 0) = u.z() * u.x() * cos_1 - u.y() * sin_theta;
  m(2, 1) = u.z() * u.y() * cos_1 + u.x() * sin_theta;
  m(2, 2) = u.z() * u.z() * cos_1 + cos_theta;

  Ensures(std::fabs(m.determinant() - 1.0) < 1e-10);
}

Rotation3d::Rotation3d(YawPitchRoll ypr) {

  const double c_1 = cos(ypr.yaw), s_1 = sin(ypr.yaw), c_2 = cos(ypr.pitch),
               s_2 = sin(ypr.pitch), c_3 = cos(ypr.roll), s_3 = sin(ypr.roll);

  m(0, 0) = c_1 * c_2;
  m(0, 1) = c_1 * s_2 * s_3 - c_3 * s_1;
  m(0, 2) = s_1 * s_3 + c_1 * c_3 * s_2;

  m(1, 0) = c_2 * s_1;
  m(1, 1) = c_1 * c_3 + s_1 * s_2 * s_3;
  m(1, 2) = c_3 * s_1 * s_2 - c_1 * s_3;

  m(2, 0) = -s_2;
  m(2, 1) = c_2 * s_3;
  m(2, 2) = c_2 * c_3;
  Ensures(std::fabs(m.determinant() - 1.0) < 1e-10);
}

Rotation3d::Rotation3d(EulerAngles ea) {

  const double c_1 = cos(ea.alpha), s_1 = sin(ea.alpha), c_2 = cos(ea.beta),
               s_2 = sin(ea.beta), c_3 = cos(ea.gamma), s_3 = sin(ea.gamma);

  m(0, 0) = c_1 * c_3 - c_2 * s_1 * s_3;
  m(0, 1) = -c_1 * s_3 - c_2 * c_3 * s_1;
  m(0, 2) = s_1 * s_2;

  m(1, 0) = c_3 * s_1 + c_1 * c_2 * s_3;
  m(1, 1) = c_1 * c_2 * c_3 - s_1 * s_3;
  m(1, 2) = -c_1 * s_2;

  m(2, 0) = s_2 * s_3;
  m(2, 1) = c_3 * s_2;
  m(2, 2) = c_2;
  Ensures(std::fabs(m.determinant() - 1.0) < 1e-15);
}

Rotation3d::Rotation3d(const Eigen::Matrix3d& m) : m(m) {
  double diff = (m*m.transpose() - Eigen::Matrix3d::Identity()).norm();
  if (GSL_UNLIKELY(diff > 1e-10)){
    throw BadRotationMatrix("m * m^T != Identity");
  }
}

YawPitchRoll Rotation3d::yaw_pitch_roll() const {
  return {Radians(atan2(m(1, 0), m(0, 0))), Radians(-asin(to_domain_11(m(2, 0)))),
          Radians(atan2(m(2, 1), m(2, 2)))};
}

EulerAngles Rotation3d::euler_angles() const {
  return {Radians(atan2(m(0, 2), -m(1, 2))), Radians(acos(to_domain_11(m(2, 2)))),
          Radians(atan2(m(2, 0), m(2, 1)))};
}

AngleValue Rotation3d::theta() const {
  return Radians(std::acos(
      to_domain_11(
          (m(0, 0) + m(1, 1) + m(2, 2) - 1) / 2.0
      )
  ));
}
XYZ Rotation3d::axis() const {

  double cos_theta = (m(0, 0) + m(1, 1) + m(2, 2) - 1) / 2.0;
  double sin_theta2 = 1 - cos_theta * cos_theta;
  double sin_theta = std::sqrt(sin_theta2 > 0.0 ? sin_theta2 : 0.0);
  if (sin_theta < 1e-15) {
    return XYZ{1, 0, 0};
  }
  return XYZ((m(2, 1) - m(1, 2)) * 0.5 / sin_theta,
             (m(0, 2) - m(2, 0)) * 0.5 / sin_theta,
             (m(1, 0) - m(0, 1)) * 0.5 / sin_theta);
}

Rotation3d& Rotation3d::operator*=(const Rotation3d& rhs) {
  m *= rhs.m;
  return *this;
}

Rotation3d Rotation3d::inverted() const {
  Rotation3d r(*this);
  std::swap(r.m(0, 1), r.m(1, 0));
  std::swap(r.m(0, 2), r.m(2, 0));
  std::swap(r.m(1, 2), r.m(2, 1));
  return r;
}

Transformation3d::Transformation3d()
    : m(Eigen::Matrix3d::Identity()), dr(0, 0, 0) {}

Transformation3d::Transformation3d(
    const xmol::geometry::Translation3d& translation3d)
    : m(Eigen::Matrix3d::Identity()), dr(translation3d.dr()) {}

Transformation3d::Transformation3d(const xmol::geometry::Rotation3d& rotation3d)
    : m(rotation3d.get_underlying_matrix()), dr(0, 0, 0) {}

Transformation3d::Transformation3d(
    const xmol::geometry::Rotation3d& rotation3d,
    const xmol::geometry::Translation3d& translation3d)
    : m(rotation3d.get_underlying_matrix()), dr(translation3d.dr()) {}

Transformation3d& Transformation3d::
operator*=(const xmol::geometry::Translation3d& rhs) {
  dr += rhs.dr();
  return *this;
}
Transformation3d& Transformation3d::
operator*=(const xmol::geometry::UniformScale3d& rhs) {
  m *= rhs.scale();
  return *this;
}
Transformation3d& Transformation3d::
operator*=(const xmol::geometry::Rotation3d& rhs) {
  auto& m2 = rhs.get_underlying_matrix();
  this->m *= m2;
  return *this;
}

Transformation3d& Transformation3d::
operator*=(const xmol::geometry::Transformation3d& M1) {
  auto& m1 = M1.m;
  dr = dr + XYZ(m * M1.dr.get_underlying_vector());
  m *= m1;
  return *this;
}

XYZ Transformation3d::transform(const xmol::geometry::XYZ& r) const {
  return XYZ(m * r.get_underlying_vector()) + dr;
}

Transformation3d Transformation3d::inverted() const {
  Transformation3d r(*this);
  std::swap(r.m(0, 1), r.m(1, 0));
  std::swap(r.m(0, 2), r.m(2, 0));
  std::swap(r.m(1, 2), r.m(2, 1));
  double scale2 = (r.m(0, 1) * r.m(0, 1) + r.m(0, 2) * r.m(0, 2) +
      r.m(0, 0) * r.m(0, 0));
  r.m /= scale2;
  Eigen::Vector3d vvv = r.m * dr.get_underlying_vector();
  r.dr = -XYZ(vvv);
  return r;
}

Transformation3d::Transformation3d(
    const xmol::geometry::UniformScale3d& uniformScale3d)
    : m(Eigen::Matrix3d::Identity() * uniformScale3d.scale()), dr(0, 0, 0) {}

XYZ Rotation3d::transform(const XYZ& r) const {
  return XYZ(Eigen::Vector3d(m * r.get_underlying_vector()));
}

Rotation3d xmol::geometry::operator*(const Rotation3d& lhs,
                                     const Rotation3d& rhs) {
  Rotation3d result(lhs);
  result *= rhs;
  return result;
}
UniformScale3d xmol::geometry::operator*(const UniformScale3d& lhs,
                                         const UniformScale3d& rhs) {
  UniformScale3d result(lhs);
  result *= rhs;
  return result;
}
Translation3d xmol::geometry::operator*(const Translation3d& lhs,
                                        const Translation3d& rhs) {
  Translation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Transformation3d& lhs,
                                           const Translation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Transformation3d& lhs,
                                           const UniformScale3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Transformation3d& lhs,
                                           const Rotation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Transformation3d& lhs,
                                           const Transformation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Translation3d& lhs,
                                           const UniformScale3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Translation3d& lhs,
                                           const Rotation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Translation3d& lhs,
                                           const Transformation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const UniformScale3d& lhs,
                                           const Translation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const UniformScale3d& lhs,
                                           const Rotation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const UniformScale3d& lhs,
                                           const Transformation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Rotation3d& lhs,
                                           const Translation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Rotation3d& lhs,
                                           const UniformScale3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
Transformation3d xmol::geometry::operator*(const Rotation3d& lhs,
                                           const Transformation3d& rhs) {
  Transformation3d result(lhs);
  result *= rhs;
  return result;
}
