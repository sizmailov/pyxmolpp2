#pragma once
#include "XYZ.h"
#include <vector>

namespace xmol{
namespace geometry{


std::vector<double> calculate_sasa(
    const std::vector<XYZ>& coords,
    const std::vector<double>& radii,
    double solvent_radii,
    int n_samples=20
    );

}
}
