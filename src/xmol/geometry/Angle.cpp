#include "xmol/geometry/Angle.h"

using namespace xmol::geometry;

Degrees xmol::geometry::literals::operator"" _deg(long double value) {
  return Degrees(value);
}

Radians xmol::geometry::literals::operator"" _rad(long double value) {
  return Radians(value);
}

double xmol::geometry::sin(const Angle& angle) {
  return std::sin(angle.radians());
}

double xmol::geometry::cos(const Angle& angle) {
  return std::cos(angle.radians());
}

double xmol::geometry::tan(const Angle& angle) {
  return std::tan(angle.radians());
}

Angle xmol::geometry::fabs(const Angle& angle) {
  return Radians(std::fabs(angle.radians()));
}
