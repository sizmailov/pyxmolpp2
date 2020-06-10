#pragma once

#include "xmol/v1/future/span.h"
#include "xmol/v1/fwd.h"
#include "xmol/v1/geom/XYZ.h"
#include <vector>

namespace xmol::v1::algo {

std::vector<double> calc_sasa(const std::vector<geom::XYZ>& coords, future::Span<double> coord_radii,
                              double solvent_radii, int n_samples = 20,
                              const future::Span<int>& sasa_points_indices = {});

}