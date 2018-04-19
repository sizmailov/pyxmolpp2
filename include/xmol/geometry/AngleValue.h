#pragma once
#include <cmath>

namespace xmol::geometry {

constexpr double degrees_to_radians(double degrees) { return degrees * (M_PI / 180.0); }
constexpr double radians_to_degrees(double radians) { return radians * (180.0 / M_PI); }

struct Degrees {
  explicit constexpr Degrees(double value) : value(value){};
  double value;
};

struct Radians {
  explicit constexpr Radians(double value) : value(value){};
  double value;
};

struct AngleValue {
  constexpr AngleValue() : m_radians(0.0){};
  AngleValue(const AngleValue&) = default;
  AngleValue(AngleValue&&) = default;
  constexpr AngleValue(const Radians& radians) : m_radians(radians.value) {}
  constexpr AngleValue(const Degrees& degrees)
      : m_radians(degrees_to_radians(degrees.value)) {}

  AngleValue& operator=(const AngleValue&) = default;
  AngleValue& operator=(AngleValue&&) = default;

  constexpr double radians() const { return m_radians; };
  constexpr double degrees() const { return radians_to_degrees(m_radians); };

  constexpr AngleValue operator+() const { return *this; }

  constexpr AngleValue operator-() const { return Radians(-m_radians); }
  constexpr AngleValue& operator+=(const AngleValue& u) {
    m_radians += u.m_radians;
    return *this;
  }
  constexpr AngleValue& operator-=(const AngleValue& u) {
    m_radians -= u.m_radians;
    return *this;
  }
  constexpr AngleValue operator+(const AngleValue& u) const {
    return Radians(m_radians + u.m_radians);
  }
  constexpr AngleValue operator-(const AngleValue& u) const {
    return Radians(m_radians - u.m_radians);
  }

  constexpr AngleValue& operator*=(const double& u) {
    m_radians *= u;
    return *this;
  }
  constexpr AngleValue& operator/=(const double& u) {
    m_radians /= u;
    return *this;
  }

  constexpr AngleValue operator/(const double& u) const {
    return Radians(m_radians / u);
  }
  constexpr AngleValue operator*(const double& u) const {
    return Radians(m_radians * u);
  }

  constexpr bool operator<(const AngleValue& u) const {
    return m_radians < u.m_radians;
  }
  constexpr bool operator<=(const AngleValue& u) const {
    return m_radians <= u.m_radians;
  }
  constexpr bool operator>(const AngleValue& u) const {
    return m_radians > u.m_radians;
  }
  constexpr bool operator>=(const AngleValue& u) const {
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

inline AngleValue operator*(double lhs, const AngleValue& rhs){
  return rhs*lhs;
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
