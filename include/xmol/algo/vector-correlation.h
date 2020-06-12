#pragma once
#include "xmol/future/span.h"
#include "xmol/fwd.h"
#include "xmol/geom/XYZ.h"
#include <vector>

namespace xmol::algo {

enum class AutoCorrelationMode { NORMALIZE_VECTORS, NORMALIZE_AND_DIVIDE_BY_CUBE };

void calc_autocorr_order_2(const future::Span<geom::XYZ>& v, future::Span<double> result,
                           const AutoCorrelationMode& mode = AutoCorrelationMode::NORMALIZE_VECTORS);

} // namespace xmol::algo