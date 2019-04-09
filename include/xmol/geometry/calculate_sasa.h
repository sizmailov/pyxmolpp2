#pragma once
#include "XYZ.h"
#include <vector>

namespace xmol{
namespace geometry{


std::vector<double> calculate_sasa(
    const std::vector<xmol::geometry::XYZ> &coords,
    double *radii_begin,
    double *radii_end,
    double solvent_radii,
    int n_samples=20,
    const int *sasa_points_indices_begin=nullptr,
    const int *sasa_points_indices_end=nullptr
    );

}
}
