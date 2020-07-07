#pragma once
#include "xmol/trajectory/Trajectory.h"
#include "xmol/trajectory/TrajectoryFile.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

class PyTrajectoryInputFile : public xmol::trajectory::TrajectoryInputFile {
public:
  [[nodiscard]] size_t n_frames() const override;
  [[nodiscard]] size_t n_atoms() const override;
  void read_frame(size_t index, xmol::Frame& frame) override;
  void advance(size_t shift) override;
};

void populate(pybind11::class_<xmol::trajectory::Trajectory>& pyTrajectory);
void populate(pybind11::class_<xmol::trajectory::TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile);

} // namespace pyxmolpp::v1