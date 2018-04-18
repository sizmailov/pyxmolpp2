#pragma once
#include <cmath>

namespace xmol::geometry {

constexpr double to_radians(double degrees) { return degrees * (M_PI / 180.0); }
constexpr double to_degrees(double radians) { return radians * (180.0 / M_PI); }

struct Degrees {
  explicit constexpr Degrees(double value) : value(value){};
  double value;
};

struct Radians {
  explicit constexpr Radians(double value) : value(value){};
  double value;
};

struct Angle {
  constexpr Angle() : m_radians(0.0){};
  Angle(const Angle&) = default;
  Angle(Angle&&) = default;
  constexpr Angle(const Radians& radians) : m_radians(radians.value) {}
  constexpr Angle(const Degrees& degrees)
      : m_radians(to_radians(degrees.value)) {}

  Angle& operator=(const Angle&) = default;
  Angle& operator=(Angle&&) = default;

  constexpr double radians() const { return m_radians; };
  constexpr double degrees() const { return to_degrees(m_radians); };

  constexpr Angle operator+() const { return *this; }

  constexpr Angle operator-() const { return Radians(-m_radians); }
  constexpr Angle& operator+=(const Angle& u) {
    m_radians += u.m_radians;
    return *this;
  }
  constexpr Angle& operator-=(const Angle& u) {
    m_radians -= u.m_radians;
    return *this;
  }
  constexpr Angle operator+(const Angle& u) const {
    return Radians(m_radians + u.m_radians);
  }
  constexpr Angle operator-(const Angle& u) const {
    return Radians(m_radians - u.m_radians);
  }

  constexpr Angle& operator*=(const double& u) {
    m_radians *= u;
    return *this;
  }
  constexpr Angle& operator/=(const double& u) {
    m_radians /= u;
    return *this;
  }

  constexpr Angle operator/(const double& u) const {
    return Radians(m_radians / u);
  }
  constexpr Angle operator*(const double& u) const {
    return Radians(m_radians * u);
  }

  constexpr bool operator<(const Angle& u) const {
    return m_radians < u.m_radians;
  }
  constexpr bool operator<=(const Angle& u) const {
    return m_radians <= u.m_radians;
  }
  constexpr bool operator>(const Angle& u) const {
    return m_radians > u.m_radians;
  }
  constexpr bool operator>=(const Angle& u) const {
    return m_radians >= u.m_radians;
  }

  Angle to_standard_range() const {
    double rad = std::fmod(m_radians, 2 * M_PI);
    if (rad < -M_PI) {
      rad += 2 * M_PI;
    }
    if (rad > M_PI) {
      rad -= 2 * M_PI;
    }
    return Radians(rad);
  }

private:
  double m_radians;
};

double sin(const Angle& angle);
double cos(const Angle& angle);
double tan(const Angle& angle);
Angle fabs(const Angle& angle);

inline namespace literals {
Degrees operator"" _deg(long double value);
Radians operator"" _rad(long double value);
}
}
