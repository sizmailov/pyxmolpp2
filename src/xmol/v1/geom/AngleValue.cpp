#include "xmol/geom/AngleValue.h"
#include "xmol/geom/XYZ.h"

using namespace xmol::geom;

Degrees xmol::geom::literals::operator"" _deg(long double value) {
  return Degrees(value);
}

Radians xmol::geom::literals::operator"" _rad(long double value) {
  return Radians(value);
}

double xmol::geom::sin(const AngleValue& angle) {
  return std::sin(angle.radians());
}

double xmol::geom::cos(const AngleValue& angle) {
  return std::cos(angle.radians());
}

double xmol::geom::tan(const AngleValue& angle) {
  return std::tan(angle.radians());
}

AngleValue xmol::geom::fabs(const AngleValue& angle) {
  return Radians(std::fabs(angle.radians()));
}

AngleValue xmol::geom::dihedral_angle(const XYZ& a, const XYZ& b, const XYZ& c, const XYZ& d) {
    XYZ ba = a - b;
    XYZ bc = c - b;
    XYZ cd = d - c;
    XYZ abc = -ba.cross(bc);
    XYZ bcd = bc.cross(cd);
    return Radians(std::atan2(abc.cross(bcd).dot(bc) / bc.len(), abc.dot(bcd)));
}
