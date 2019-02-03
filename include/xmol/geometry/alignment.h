#pragma once

#include "Transformation3d.h"
#include <vector>

namespace xmol {
namespace geometry {

Transformation3d calc_alignment(const std::vector<XYZ>& reference,
                                const std::vector<XYZ>& variable);

Eigen::Matrix3d calc_inertia_tensor(const std::vector<XYZ>& coords);

Eigen::Matrix3d calc_inertia_tensor(const std::vector<XYZ>& coords, const std::vector<double>& mass);

XYZ calc_mass_center(const std::vector<XYZ>& coordinates, const std::vector<double>& mass);

XYZ calc_geom_center(const std::vector<XYZ>& coordinates);

double calc_rmsd(const std::vector<XYZ>& reference,
                 const std::vector<XYZ>& variable,
                 const Transformation3d& alignment);

double calc_rmsd(const std::vector<XYZ>& reference,
                 const std::vector<XYZ>& variable);
}
}
