#pragma once
#include "fwd.h"
#include "xmol/future/span.h"
#include <map>
#include <vector>

namespace xmol::geom {

class SpatialIndex {
public:
  using bin_id_t = std::tuple<int, int, int>;
  using index_t = int;
  using indices_t = std::vector<index_t>;

  SpatialIndex(const future::Span<XYZ>& coords, double bin_side_length);
  indices_t within(double distance, const XYZ& point);

private:
  std::map<bin_id_t, indices_t> m_bins;
  const future::Span<XYZ>& m_coords;
  const double m_bin_side_length = 1.0;
};

} // namespace xmol::geom