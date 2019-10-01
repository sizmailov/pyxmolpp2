#pragma once

#include "XYZ.h"
#include <complex>
#include <vector>

namespace xmol {
namespace geometry {

enum class AutoCorrelationMode {
  NORMALIZE_VECTORS,
  NORMALIZE_AND_DIVIDE_BY_CUBE
};

std::vector<double> calc_autocorr_order_2(
    const std::vector<XYZ> &v,
    long long limit = -1,
    const AutoCorrelationMode &mode = AutoCorrelationMode::NORMALIZE_VECTORS
);

}
}
