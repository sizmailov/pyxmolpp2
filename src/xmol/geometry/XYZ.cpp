#include "xmol/geometry/XYZ.h"

using namespace xmol::geometry;

std::ostream& xmol::geometry::operator<<(std::ostream& out, const XYZ& r) {
  out << "[" << std::setw(8) << std::setprecision(6) << r.x() << ", " << std::setw(8) << std::setprecision(6) << r.y()
      << ", " << std::setw(8) << std::setprecision(6) << r.z() << "]";
  return out;
}

XYZ xmol::geometry::lower_bound(const XYZ& a, const XYZ& b) {
  return XYZ(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

XYZ xmol::geometry::upper_bound(const XYZ& a, const XYZ& b) {
  return XYZ(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::min(a.z(), b.z()));
}