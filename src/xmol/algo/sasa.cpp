#include "xmol/algo/sasa.h"
#include "xmol/geom/SpatialIndex.h"
#include <algorithm>
#include <numeric>

#include <gsl/assert>

using namespace xmol::geom;

void xmol::algo::calc_sasa(const future::Span<geom::XYZ>& coords, future::Span<double> coord_radii,
                           double solvent_radii, future::Span<double> result, int n_samples,
                           const future::Span<int>& sasa_points_indices) {
  auto limit = sasa_points_indices.empty() ? coords.size() : sasa_points_indices.size();
  if (coords.size() != coord_radii.size()) {
    throw GeomError("xmol::algo::calc_sasa: coords.size() != radii.size()");
  }
  if (!sasa_points_indices.empty() && coords.size() < sasa_points_indices.size()) {
    throw GeomError("xmol::algo::calc_sasa: coords.size() < sasa_points_indices.size()");
  }
  if (limit != result.size()) {
    throw GeomError("xmol::algo::calc_sasa: result.size() != limit");
  }
  const double max_radii = std::accumulate(coord_radii.begin(), coord_radii.end(), 0.0,
                                           [](const double& a, const double& b) { return std::max(a, b); });
  const double neighbour_cell_size = (max_radii + solvent_radii) * 2;

  SpatialIndex spatial_index(coords, neighbour_cell_size);

  auto segments_length = [](std::vector<std::pair<double, double>>& segments) {
    double result = 0;
    std::sort(segments.begin(), segments.end());
    auto it = segments.begin();
    if (it == segments.end()) {
      return result;
    }
    auto prev = *it;
    ++it;
    while (it != segments.end()) {
      if (it->first > prev.second) {
        result += prev.second - prev.first;
        prev = *it;
      } else {
        prev.second = std::max(prev.second, it->second);
      }
      ++it;
    }
    result += prev.second - prev.first;
    return result;
  };

  for (int i1 = 0; i1 < limit; ++i1) {
    int n = sasa_points_indices.empty() ? i1 : sasa_points_indices[i1];
    if (GSL_UNLIKELY(n < 0 && n >= coords.size())) {
      throw GeomError("xmol::geometry::calculate_sasa: invalid index `" + std::to_string(n) + "`");
    }

    double Rn = coord_radii[n] + solvent_radii;
    double atom_area = 4 * M_PI * Rn * Rn;

    double delta = 2 * Rn / n_samples;
    double max_distance = (coord_radii[n] + max_radii + 2 * solvent_radii);
    std::vector<int> neigh_indecies = spatial_index.within(max_distance, coords[n]);

    for (int slice_i = 0; slice_i < n_samples; ++slice_i) {
      double dz = -Rn + delta / 2 + delta * slice_i;
      double Rn_ = std::sqrt(Rn * Rn - dz * dz);
      std::vector<std::pair<double, double>> segments;

      for (int m : neigh_indecies) {
        if (m == n) {
          continue;
        }
        double Rm = coord_radii[m] + solvent_radii;
        double dz2 = coords[n].z() - coords[m].z() + dz;
        if (std::fabs(dz2) >= Rm) {
          continue;
        }
        double Rm_ = std::sqrt(Rm * Rm - dz2 * dz2);
        double dx = coords[m].x() - coords[n].x();
        double dy = coords[m].y() - coords[n].y();
        double d = std::sqrt(dx * dx + dy * dy);

        if (d >= Rn_ + Rm_) {
          continue;
        } // no overlap
        if (Rn_ >= d + Rm_) {
          continue;
        }                     // second circle within first circle
        if (Rm_ >= d + Rn_) { // first circle within second circle
          segments.emplace_back(-M_PI, M_PI);
          continue;
        }

        double alpha = std::acos((Rn_ * Rn_ + d * d - Rm_ * Rm_) / (2 * Rn_ * d));
        double beta = std::atan2(dy, dx);

        double l = beta - alpha;
        double r = beta + alpha;

        if (-M_PI < l && r < M_PI) {
          segments.emplace_back(l, r);
        } else if (l < -M_PI) {
          segments.emplace_back(l + M_PI * 2, M_PI);
          segments.emplace_back(-M_PI, r);
        } else {
          segments.emplace_back(l, M_PI);
          segments.emplace_back(-M_PI, r - M_PI * 2);
        }
      }
      atom_area -= segments_length(segments) * Rn * delta;
    }
    result[i1] = atom_area;
  }
}
