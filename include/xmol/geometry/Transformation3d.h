#pragma once

#include "AngleValue.h"
#include "XYZ.h"

namespace xmol {
namespace geometry {

struct YawPitchRoll {
  AngleValue yaw;
  AngleValue pitch;
  AngleValue roll;
};

struct EulerAngles {
  AngleValue alpha;
  AngleValue beta;
  AngleValue gamma;
};

class Translation3d {
public:
  Translation3d() : m_dr(0, 0, 0){};

  explicit Translation3d(const XYZ& dr) : m_dr(dr){};

  XYZ transform(const XYZ& r) { return r + m_dr; };

  Translation3d& operator*=(const Translation3d& rhs) {
    m_dr += rhs.m_dr;
    return *this;
  }

  const XYZ& dr() const { return m_dr; }

  Translation3d inverted() const { return Translation3d(-m_dr); }

private:
  XYZ m_dr;
};

class UniformScale3d {
public:
  UniformScale3d() : UniformScale3d(1.0){};

  explicit UniformScale3d(double scale) : m_scale(scale){};
  UniformScale3d(UniformScale3d&&) = default;
  UniformScale3d(const UniformScale3d&) = default;
  UniformScale3d& operator=(const UniformScale3d&) = default;
  UniformScale3d& operator=(UniformScale3d&&) = default;

  XYZ transform(const XYZ& r) { return r * m_scale; };

  UniformScale3d& operator*=(const UniformScale3d& rhs) {
    m_scale *= rhs.m_scale;
    return *this;
  }

  double scale() const { return m_scale; }

  UniformScale3d inverted() const { return UniformScale3d(1 / m_scale); }

private:
  double m_scale;
};

class Rotation3d {
public:
  Rotation3d();
  Rotation3d(XYZ axis, AngleValue theta);
  explicit Rotation3d(YawPitchRoll rollPitchYaw);
  explicit Rotation3d(EulerAngles ea);
  explicit Rotation3d(const Eigen::Matrix3d& m);

  YawPitchRoll yaw_pitch_roll() const;
  EulerAngles euler_angles() const;

  AngleValue theta() const;
  XYZ axis() const;

  XYZ transform(const XYZ& r) const;
  Rotation3d& operator*=(const Rotation3d& rhs);

  Rotation3d inverted() const;

  const Eigen::Matrix3d& get_underlying_matrix() const { return m; }

  Eigen::Matrix3d& get_underlying_matrix() { return m; }

private:
  Eigen::Matrix3d m;
};

class Transformation3d {
public:
  Transformation3d();
  explicit Transformation3d(const Translation3d& translation3d);
  explicit Transformation3d(const Rotation3d& rotation3d);
  explicit Transformation3d(const UniformScale3d& uniformScale3d);
  Transformation3d(const Rotation3d& rotation3d,
                   const Translation3d& translation3d);
  Transformation3d(const Transformation3d& translation3d) = default;
  Transformation3d(Transformation3d&& translation3d) = default;
  Transformation3d& operator=(const Transformation3d& translation3d) = default;
  Transformation3d& operator=(Transformation3d&& translation3d) = default;

  Transformation3d& operator*=(const Translation3d& rhs);
  Transformation3d& operator*=(const UniformScale3d& rhs);
  Transformation3d& operator*=(const Rotation3d& rhs);
  Transformation3d& operator*=(const Transformation3d& M1);

  XYZ transform(const XYZ& r) const;
  Transformation3d inverted() const;


  const Eigen::Matrix3d& get_underlying_matrix() const { return m; }
  Eigen::Matrix3d& get_underlying_matrix() { return m; }

  XYZ get_translation() const { return dr;}

private:
  Eigen::Matrix3d m;
  XYZ dr;
};

Rotation3d operator*(const Rotation3d&, const Rotation3d& m);
UniformScale3d operator*(const UniformScale3d&, const UniformScale3d& m);
Translation3d operator*(const Translation3d&, const Translation3d& m);

Transformation3d operator*(const Transformation3d&, const Translation3d&);
Transformation3d operator*(const Transformation3d&, const UniformScale3d&);
Transformation3d operator*(const Transformation3d&, const Rotation3d&);
Transformation3d operator*(const Transformation3d&, const Transformation3d&);

Transformation3d operator*(const Translation3d&, const UniformScale3d&);
Transformation3d operator*(const Translation3d&, const Rotation3d&);
Transformation3d operator*(const Translation3d&, const Transformation3d&);

Transformation3d operator*(const UniformScale3d&, const Translation3d&);
Transformation3d operator*(const UniformScale3d&, const Rotation3d&);
Transformation3d operator*(const UniformScale3d&, const Transformation3d&);

Transformation3d operator*(const Rotation3d&, const Translation3d&);
Transformation3d operator*(const Rotation3d&, const UniformScale3d&);
Transformation3d operator*(const Rotation3d&, const Transformation3d&);
}
}