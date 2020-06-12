#pragma once
#include "xmol/fwd.h"
#include "xmol/future/span.h"
#include "xmol/geom/XYZ.h"
#include <vector>

namespace xmol::algo {

enum class AutoCorrelationMode { NORMALIZE_VECTORS, NORMALIZE_AND_DIVIDE_BY_CUBE };

std::vector<double> calc_autocorr_order_2(const future::Span<geom::XYZ>& v, long long limit = -1,
    const AutoCorrelationMode &mode = AutoCorrelationMode::NORMALIZE_VECTORS
);

} // namespace xmol::geom