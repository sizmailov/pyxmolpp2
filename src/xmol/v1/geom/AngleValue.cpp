#include "xmol/geometry/AngleValue.h"
#include "xmol/geometry/XYZ.h"

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

AngleValue xmol::v1::geom::dihedral_angle(const XYZ& a, const XYZ& b, const XYZ& c, const XYZ& d) {
    XYZ ba = a - b;
    XYZ bc = c - b;
    XYZ cd = d - c;
    XYZ abc = -ba.cross(bc);
    XYZ bcd = bc.cross(cd);
    return Radians(std::atan2(abc.cross(bcd).dot(bc) / bc.len(), abc.dot(bcd)));
}
