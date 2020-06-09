#pragma once

#include "AngleValue.h"
#include "XYZ.h"
#include "gsl/gsl_assert"

namespace xmol {
namespace geometry {

inline double distance(const XYZ& a, const XYZ& b) { return (a - b).len(); }

inline double distance2(const XYZ& a, const XYZ& b) { return (a - b).len2(); }


}
}
