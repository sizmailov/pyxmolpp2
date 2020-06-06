#pragma once
#include "xmol/v1/future/span.h"
#include "xmol/v1/fwd.h"

namespace xmol::v1::trajectory {

class TrajectoryInputFile {
public:
  [[nodiscard]] virtual size_t n_frames() const = 0;
  [[nodiscard]] virtual size_t n_atoms() const = 0;
  virtual void read_coordinates(size_t index, future::Span<XYZ>& coordinates) = 0;

  virtual void advance(size_t shift) = 0;
};

} // namespace xmol::v1::trajectory