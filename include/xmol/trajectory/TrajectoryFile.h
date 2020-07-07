#pragma once
#include "xmol/proxy/spans.h"
#include "xmol/fwd.h"
#include "xmol/geom/fwd.h"

namespace xmol::trajectory {

/// Forward read-only re-enterable trajectory coordinate file
class TrajectoryInputFile {
public:
  virtual ~TrajectoryInputFile() = default;
  /// Number of frames
  [[nodiscard]] virtual size_t n_frames() const = 0;

  /// Number of atoms per frame
  [[nodiscard]] virtual size_t n_atoms() const = 0;

  /** Read @p index 'th  frame into coordinates
   *
   * Note: trajectory without cell info should ignore `cell` argument.
   * Precondition: index must match current position of internal data pointer
   * */
  virtual void read_frame(size_t index, Frame& frame) = 0;

  /** Advance internal data pointer by @p shift frames and be prepared to read coordinates
   *
   * When internal data pointer shifted beyond @ref n_frames() file handles must be closed
   *
   * Note: advance(0) prepares file to be read by @ref read_coordinates()
   * */
  virtual void advance(size_t shift) = 0;

};

} // namespace xmol::trajectory