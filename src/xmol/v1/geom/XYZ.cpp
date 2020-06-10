#include "xmol/v1/geom/XYZ.h"
#include "xmol/v1/geom/AngleValue.h"

using namespace xmol::v1::geom;

std::ostream& xmol::v1::geom::operator<<(std::ostream& out, const XYZ& r) {
  out << "[" << std::setw(8) << std::setprecision(6) << r.x() << ", " << std::setw(8) << std::setprecision(6) << r.y()
      << ", " << std::setw(8) << std::setprecision(6) << r.z() << "]";
  return out;
}

XYZ xmol::v1::geom::lower_bound(const XYZ& a, const XYZ& b) {
  return XYZ(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

XYZ xmol::v1::geom::upper_bound(const XYZ& a, const XYZ& b) {
  return XYZ(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::min(a.z(), b.z()));
}

AngleValue XYZ::angle(const XYZ& other) const { return Radians(std::acos(dot(other))); }

AngleValue XYZ::angle_between(const XYZ& a, const XYZ& c) const {
  XYZ ba = a - *this;
  XYZ bc = c - *this;
  return Radians(std::acos(std::max(-1.0, std::min(1.0, ba.dot(bc) / (ba.len() * bc.len())))));
}
