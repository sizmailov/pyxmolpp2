#include "xmol/geom/SpatialIndex.h"
#include "xmol/geom/XYZ.h"
#include <cmath>

using namespace xmol::geom;

SpatialIndex::SpatialIndex(const xmol::future::Span<XYZ>& coords, double bin_side_length)
    : m_coords(coords), m_bin_side_length(bin_side_length) {
  auto to_bin_id = [&](double x) { return int(std::round(x / m_bin_side_length)); };
  for (int i = 0; i < coords.size(); ++i) {
    auto& r = coords[i];
    m_bins[{to_bin_id(r.x()), to_bin_id(r.y()), to_bin_id(r.z())}].push_back(i);
  }
}
SpatialIndex::indices_t SpatialIndex::within(double distance, const XYZ& point) {
  auto to_bin_id = [&](double x) { return int(std::round(x / m_bin_side_length)); };
  indices_t result;

  const int n_neigh_cells = std::ceil(distance / m_bin_side_length);

  int i = to_bin_id(point.x());
  int j = to_bin_id(point.y());
  int k = to_bin_id(point.z());

  double distance2 = distance * distance;

  for (int di = -n_neigh_cells; di <= n_neigh_cells; ++di) {
    for (int dj = -n_neigh_cells; dj <= n_neigh_cells; ++dj) {
      for (int dk = -n_neigh_cells; dk <= n_neigh_cells; ++dk) {
        auto it = m_bins.find(std::make_tuple(i + di, j + dj, k + dk));
        if (it == m_bins.end()) {
          continue;
        }
        for (int m : it->second) {
          if (point.distance2(m_coords[m]) < distance2) {
            result.push_back(m);
          }
        }
      }
    }
  }
  return result;
}
