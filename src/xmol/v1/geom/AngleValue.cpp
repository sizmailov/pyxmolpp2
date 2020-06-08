#include "xmol/geometry/AngleValue.h"

using namespace xmol::v1::geom;

Degrees xmol::v1::geom::literals::operator"" _deg(long double value) {
  return Degrees(value);
}

Radians xmol::v1::geom::literals::operator"" _rad(long double value) {
  return Radians(value);
}

double xmol::v1::geom::sin(const AngleValue& angle) {
  return std::sin(angle.radians());
}

double xmol::v1::geom::cos(const AngleValue& angle) {
  return std::cos(angle.radians());
}

double xmol::v1::geom::tan(const AngleValue& angle) {
  return std::tan(angle.radians());
}

AngleValue xmol::v1::geom::fabs(const AngleValue& angle) {
  return Radians(std::fabs(angle.radians()));
}
