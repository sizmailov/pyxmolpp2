#pragma once

#include "AngleValue.h"
#include "XYZ.h"
#include "gsl/gsl_assert"

namespace xmol {
namespace geometry {

inline double distance(const XYZ& a, const XYZ& b) { return (a - b).len(); }

inline double distance2(const XYZ& a, const XYZ& b) { return (a - b).len2(); }

inline AngleValue angle(const XYZ& a, const XYZ& b, const XYZ& c) {
  XYZ ba = a - b;
  XYZ bc = c - b;
  return Radians(std::acos(ba.dot(bc) / (ba.len() * bc.len())));
}

inline AngleValue dihedral_angle(const XYZ& a, const XYZ& b, const XYZ& c,
                                 const XYZ& d) {
  XYZ ba = a - b;
  XYZ bc = c - b;
  XYZ cd = d - c;
  XYZ abc = -ba.cross(bc);
  XYZ bcd = bc.cross(cd);
  return Radians(std::atan2(abc.cross(bcd).dot(bc) / bc.len(), abc.dot(bcd)));
}
}
}