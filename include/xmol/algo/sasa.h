#pragma once

#include "xmol/future/span.h"
#include "xmol/fwd.h"
#include "xmol/geom/XYZ.h"
#include <vector>

namespace xmol::algo {

void calc_sasa(const future::Span<geom::XYZ>& coords, future::Span<double> coord_radii, double solvent_radii,
               future::Span<double> result, int n_samples = 20, const future::Span<int>& sasa_points_indices = {});

}