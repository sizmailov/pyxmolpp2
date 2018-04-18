#pragma once
#include "Eigen/Dense"

namespace xmol::geometry {

class XYZ {
public:
  constexpr XYZ(double x, double y, double z) : m_v(x, y, z) {}
  constexpr XYZ() = default;
  constexpr XYZ(const XYZ& other) = default;
  constexpr XYZ(XYZ&& other) = default;
  constexpr XYZ& operator=(const XYZ& other) = default;
  constexpr XYZ& operator=(XYZ&& other) = default;

  constexpr XYZ& operator+=(const XYZ& other) {
    m_v += other.m_v;
    return *this;
  }
  constexpr XYZ& operator-=(const XYZ& other) {
    m_v -= other.m_v;
    return *this;
  }

  constexpr XYZ& operator*=(double t) {
    m_v *= t;
    return *this;
  }
  constexpr XYZ& operator/=(double t) {
    m_v /= t;
    return *this;
  }

  constexpr double len2() const { return m_v.squaredNorm(); }
  constexpr double len() const { return m_v.norm(); }

  constexpr double dot(const XYZ& b) const { return m_v.dot(b.m_v); }
  constexpr XYZ cross(const XYZ& b) const { return XYZ(m_v.cross(b.m_v)); }
  constexpr XYZ operator-() const { return XYZ(-m_v); }
  const double& x() const { return m_v[0]; }
  const double& y() const { return m_v[1]; }
  const double& z() const { return m_v[2]; }
  XYZ& set_x(double value) const {
    m_v[0] = value;
    return *this;
  }
  XYZ& set_y(double value) const {
    m_v[1] = value;
    return *this;
  }
  XYZ& set_z(double value) const {
    m_v[2] = value;
    return *this;
  }

private:
  explicit XYZ(const Eigen::Vector3d& v) : m_v(v) {}
  Eigen::Vector3d m_v;
};

constexpr XYZ operator-(const XYZ& a, const XYZ& b) {
  XYZ c(a);
  c -= b;
  return c;
};
constexpr XYZ operator+(const XYZ& a, const XYZ& b) {
  XYZ c(a);
  c += b;
  return c;
};
constexpr XYZ operator*(const XYZ& a, double t) {
  XYZ result(a);
  result *= t;
  return result;
}
constexpr XYZ operator/(const XYZ& a, double t) {
  XYZ result(a);
  result /= t;
  return result;
}
constexpr XYZ operator*(double t, const XYZ& a) {
  XYZ result(a);
  result *= t;
  return result;
}
}
