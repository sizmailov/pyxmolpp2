#pragma once
#include "xmol/v1/fwd.h"
#include "xmol/v1/geom/XYZ.h"
#include <vector>

namespace xmol::v1::algo {

enum class AutoCorrelationMode { NORMALIZE_VECTORS, NORMALIZE_AND_DIVIDE_BY_CUBE };

std::vector<double> calc_autocorr_order_2(const std::vector<geom::XYZ>& v, long long limit = -1,
    const AutoCorrelationMode &mode = AutoCorrelationMode::NORMALIZE_VECTORS
);

} // namespace xmol::v1::geom