#pragma once
#include <cmath>

namespace xmol {
namespace geometry {

inline double degrees_to_radians(double degrees) {
  return degrees * (M_PI / 180.0);
}
inline double radians_to_degrees(double radians) {
  return radians * (180.0 / M_PI);
}

struct Degrees {
  explicit inline Degrees(double value) : value(value){};
  double value;
};

struct Radians {
  explicit inline Radians(double value) : value(value){};
  double value;
};

struct AngleValue {
  inline AngleValue() : m_radians(0.0){};
  AngleValue(const AngleValue&) = default;
  AngleValue(AngleValue&&) = default;
  inline AngleValue(const Radians& radians) : m_radians(radians.value) {}
  inline AngleValue(const Degrees& degrees)
      : m_radians(degrees_to_radians(degrees.value)) {}

  AngleValue& operator=(const AngleValue&) = default;
  AngleValue& operator=(AngleValue&&) = default;

  inline double radians() const { return m_radians; };
  inline double degrees() const { return radians_to_degrees(m_radians); };

  inline AngleValue operator+() const { return *this; }

  inline AngleValue operator-() const { return Radians(-m_radians); }
  inline AngleValue& operator+=(const AngleValue& u) {
    m_radians += u.m_radians;
    return *this;
  }
  inline AngleValue& operator-=(const AngleValue& u) {
    m_radians -= u.m_radians;
    return *this;
  }
  inline AngleValue operator+(const AngleValue& u) const {
    return Radians(m_radians + u.m_radians);
  }
  inline AngleValue operator-(const AngleValue& u) const {
    return Radians(m_radians - u.m_radians);
  }

  inline AngleValue& operator*=(const double& u) {
    m_radians *= u;
    return *this;
  }
  inline AngleValue& operator/=(const double& u) {
    m_radians /= u;
    return *this;
  }

  inline AngleValue operator/(const double& u) const {
    return Radians(m_radians / u);
  }
  inline AngleValue operator*(const double& u) const {
    return Radians(m_radians * u);
  }

  inline bool operator<(const AngleValue& u) const {
    return m_radians < u.m_radians;
  }
  inline bool operator<=(const AngleValue& u) const {
    return m_radians <= u.m_radians;
  }
  inline bool operator>(const AngleValue& u) const {
    return m_radians > u.m_radians;
  }
  inline bool operator>=(const AngleValue& u) const {
    return m_radians >= u.m_radians;
  }

  AngleValue to_standard_range() const {
    double rad = std::fmod(m_radians, 2 * M_PI);
    while (rad < -M_PI) {
      rad += 2 * M_PI;
    }
    while (rad > M_PI) {
      rad -= 2 * M_PI;
    }
    return Radians(rad);
  }

private:
  double m_radians;
};

inline AngleValue operator*(double lhs, const AngleValue& rhs) {
  return rhs * lhs;
}

double sin(const AngleValue& angle);
double cos(const AngleValue& angle);
double tan(const AngleValue& angle);
AngleValue fabs(const AngleValue& angle);

inline namespace literals {
Degrees operator"" _deg(long double value);
Radians operator"" _rad(long double value);
}
}
}