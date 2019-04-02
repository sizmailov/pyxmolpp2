#include "xmol/geometry/calculate_sasa.h"
#include <map>
#include <numeric>
#include <xmol/geometry/exceptions.h>
#include <iostream>

std::vector<double> xmol::geometry::calculate_sasa(
    const std::vector<xmol::geometry::XYZ>& coords,
    const std::vector<double>& radii,
    double solvent_radii,
    int n_samples)
{
  if (coords.size()!=radii.size()) {
    throw xmol::geometry::GeometryException("xmol::geometry::calculate_sasa: coords.size()!=radii.size()");
  }
  const double max_radii = std::accumulate(radii.begin(), radii.end(), 0,
      [](const double& a, const double& b) { return std::max(a, b); });
  const double neighbour_cell_size = max_radii+solvent_radii;

  std::map<std::tuple<int, int, int>,
           std::vector<int>> neighbour_index;

  auto cell_index = [neighbour_cell_size](const XYZ& r) -> std::tuple<int, int, int> {
    return std::make_tuple(
        int(std::round(r.x()/neighbour_cell_size)),
        int(std::round(r.y()/neighbour_cell_size)),
        int(std::round(r.z()/neighbour_cell_size))
    );
  };

  auto segments_length = [](const std::vector<std::pair<double, double>>& segments) {
    double result = 0;
    auto it = segments.begin();
    if (it==segments.end()) {
      return result;
    }
    auto prev = *it;
    ++it;
    while (it!=segments.end()) {
      if (it->first>prev.second) {
        result += prev.second-prev.first;
        prev = *it;
      }
      else {
        prev.second = std::max(prev.second, it->second);
      }
      ++it;
    }
    result += prev.second-prev.first;
    return result;
  };

  for (int i = 0; i<coords.size(); ++i) {
    neighbour_index[cell_index(coords[i])].push_back(i);
  }

  for (auto& it: neighbour_index) {
    int i, j, k;
    std::tie(i, j, k) = it.first;
  }

  std::vector<double> result;
  result.reserve(coords.size());

  const int n_neigh_cells = std::ceil((2*(max_radii+solvent_radii)/neighbour_cell_size));

  for (int n = 0; n<coords.size(); ++n) {
    double Rn = radii[n]+solvent_radii;
    double atom_area = 4*M_PI*Rn*Rn;

    double delta = 2*Rn/n_samples;
    int i, j, k;
    std::tie(i, j, k) = cell_index(coords[n]);
    for (int slice_i = 0; slice_i<n_samples; ++slice_i) {
      double dz = -Rn+delta/2+delta*slice_i;
      double Rn_ = std::sqrt(Rn*Rn-dz*dz);
      std::vector<std::pair<double, double>> segments;
      for (int di = -n_neigh_cells; di<=n_neigh_cells; ++di) {
        for (int dj = -n_neigh_cells; dj<=n_neigh_cells; ++dj) {
          for (int dk = -n_neigh_cells; dk<=n_neigh_cells; ++dk) {
            auto it = neighbour_index.find(
                std::make_tuple(
                    i+di,
                    j+dj,
                    k+dk
                ));
            if (it==neighbour_index.end()) { continue; }

            for (int m: it->second) {
              if (m==n){ continue; }
              double Rm = radii[m]+solvent_radii;
              double dz2 = coords[n].z()-coords[m].z()+dz;
              if (std::fabs(dz2)>=Rm) { continue; }
              double Rm_ = std::sqrt(Rm*Rm-dz2*dz2);
              double dx = coords[m].x()-coords[n].x();
              double dy = coords[m].y()-coords[n].y();
              double d = std::sqrt(dx*dx+dy*dy);

              if (d>=Rn_+Rm_) { continue; } // no overlap
              if (Rn_>=d+Rm_) { continue; } // second circle within first circle
              if (Rm_>=d+Rn_) {// first circle within second circle
                segments.emplace_back(-M_PI, M_PI);
                continue;
              }

              double alpha = std::acos((Rn_*Rn_+d*d-Rm_*Rm_)/(2*Rn_*d));
              double beta = std::atan2(dy, dx);

              double l = beta-alpha;
              double r = beta+alpha;

              if (-M_PI<l && r<M_PI) {
                segments.emplace_back(l, r);
              }
              else if (l<-M_PI) {
                segments.emplace_back(l+M_PI*2, M_PI);
                segments.emplace_back(-M_PI, r);
              }
              else {
                segments.emplace_back(l, M_PI);
                segments.emplace_back(-M_PI, r-M_PI*2);
              }

            }

          }
        }
      }
      atom_area -= segments_length(segments)*Rn*delta;
    }
    result.push_back(atom_area);
  }
  return result;
}
