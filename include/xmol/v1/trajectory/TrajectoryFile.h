#pragma once
#include "xmol/v1/future/span.h"
#include "xmol/v1/fwd.h"

namespace xmol::v1::trajectory {

class TrajectoryInputFile {
public:
  [[nodiscard]] virtual size_t n_frames() const = 0;
  [[nodiscard]] virtual size_t n_atoms() const = 0;
  virtual size_t current_frame_index() const = 0;
  virtual void read_coordinates(size_t index, future::Span<XYZ>& coordinates) = 0;

  void advance(size_t n) {
    assert(n == current_frame_index());
    advance_impl(n);
  }

protected:
  virtual void advance_impl(int n) = 0;
};

} // namespace xmol::v1::traj