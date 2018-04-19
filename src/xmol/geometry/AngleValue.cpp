#include "xmol/geometry/AngleValue.h"

using namespace xmol::geometry;

Degrees xmol::geometry::literals::operator"" _deg(long double value) {
  return Degrees(value);
}

Radians xmol::geometry::literals::operator"" _rad(long double value) {
  return Radians(value);
}

double xmol::geometry::sin(const AngleValue& angle) {
  return std::sin(angle.radians());
}

double xmol::geometry::cos(const AngleValue& angle) {
  return std::cos(angle.radians());
}

double xmol::geometry::tan(const AngleValue& angle) {
  return std::tan(angle.radians());
}

AngleValue xmol::geometry::fabs(const AngleValue& angle) {
  return Radians(std::fabs(angle.radians()));
}
