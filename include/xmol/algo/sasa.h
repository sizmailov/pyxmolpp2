#pragma once

#include "xmol/future/span.h"
#include "xmol/fwd.h"
#include "xmol/geom/XYZ.h"
#include <vector>

namespace xmol::algo {

std::vector<double> calc_sasa(const std::vector<geom::XYZ>& coords, future::Span<double> coord_radii,
                              double solvent_radii, int n_samples = 20,
                              const future::Span<int>& sasa_points_indices = {});

}