#pragma once

#include "XYZ.h"
#include <complex>
#include <vector>

namespace xmol{
namespace geometry{

std::vector<double> calc_autocorr_order_2(const std::vector<XYZ>& v, long long limit=-1);

}
}
