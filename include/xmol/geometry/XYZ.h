#pragma once
#include "Eigen/Dense"

namespace xmol {
namespace geometry {

class XYZ {
public:
  explicit inline XYZ(const Eigen::Vector3d& v) : m_v(v) {}

  inline XYZ(double x, double y, double z) : m_v(x, y, z) {}

  inline XYZ() : XYZ(0, 0, 0){};
  inline XYZ(const XYZ& other) = default;
  inline XYZ(XYZ&& other) = default;
  inline XYZ& operator=(const XYZ& other) = default;
  inline XYZ& operator=(XYZ&& other) = default;

  inline XYZ& operator+=(const XYZ& other) {
    m_v += other.m_v;
    return *this;
  }

  inline XYZ& operator-=(const XYZ& other) {
    m_v -= other.m_v;
    return *this;
  }

  inline XYZ& operator*=(double t) {
    m_v *= t;
    return *this;
  }

  inline XYZ& operator/=(double t) {
    m_v /= t;
    return *this;
  }

  inline double len2() const { return m_v.squaredNorm(); }

  inline double len() const { return m_v.norm(); }

  inline double dot(const XYZ& b) const { return m_v.dot(b.m_v); }

  inline XYZ cross(const XYZ& b) const { return XYZ(m_v.cross(b.m_v)); }

  inline XYZ operator-() const { return XYZ(-m_v); }

  inline double x() const { return m_v[0]; }

  inline double y() const { return m_v[1]; }

  inline double z() const { return m_v[2]; }

  inline XYZ& set_x(double value) {
    m_v[0] = value;
    return *this;
  }

  inline XYZ& set_y(double value) {
    m_v[1] = value;
    return *this;
  }

  inline XYZ& set_z(double value) {
    m_v[2] = value;
    return *this;
  }

  inline const Eigen::Vector3d& get_underlying_vector() const { return m_v; }

  inline Eigen::Vector3d& get_underlying_vector() { return m_v; }

private:
  Eigen::Vector3d m_v;
};

inline XYZ operator-(const XYZ& a, const XYZ& b) {
  XYZ c(a);
  c -= b;
  return c;
};

inline XYZ operator+(const XYZ& a, const XYZ& b) {
  XYZ c(a);
  c += b;
  return c;
};

inline XYZ operator*(const XYZ& a, double t) {
  XYZ result(a);
  result *= t;
  return result;
}

inline XYZ operator/(const XYZ& a, double t) {
  XYZ result(a);
  result /= t;
  return result;
}

inline XYZ operator*(double t, const XYZ& a) {
  XYZ result(a);
  result *= t;
  return result;
}
}
}