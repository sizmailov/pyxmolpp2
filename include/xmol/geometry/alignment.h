#pragma once

#include "Transformation3d.h"
#include <vector>

namespace xmol {
namespace geometry {

Transformation3d calc_alignment(const std::vector<XYZ>& reference,
                                const std::vector<XYZ>& variable);

XYZ calc_geom_center(const std::vector<XYZ>& coordinates);

double calc_rmsd(const std::vector<XYZ>& reference,
                 const std::vector<XYZ>& variable,
                 const Transformation3d& alignment);

double calc_rmsd(const std::vector<XYZ>& reference,
                 const std::vector<XYZ>& variable);
}
}