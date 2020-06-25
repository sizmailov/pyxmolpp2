#pragma once
#include "xmol/trajectory/Trajectory.h"
#include "xmol/trajectory/TrajectoryFile.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

class PyTrajectoryInputFile : public xmol::trajectory::TrajectoryInputFile {
public:
  /* Trampoline (need one for each virtual function) */
  [[nodiscard]] size_t n_frames() const override {
    PYBIND11_OVERLOAD_PURE(size_t,              /* Return type */
                           TrajectoryInputFile, /* Parent class */
                           n_frames             /* Name of function in C++ (must match Python name) */
    );
  }
  [[nodiscard]] size_t n_atoms() const override {
    PYBIND11_OVERLOAD_PURE(size_t,              /* Return type */
                           TrajectoryInputFile, /* Parent class */
                           n_atoms              /* Name of function in C++ (must match Python name) */
    );
  }

  void read_coordinates(size_t index, xmol::proxy::CoordSpan& coordinates) override {
    PYBIND11_OVERLOAD_PURE(void,                /* Return type */
                           TrajectoryInputFile, /* Parent class */
                           read_coordinates,    /* Name of function in C++ (must match Python name) */
                           index, coordinates   /* Arguments */
    );
  }

  void advance(size_t shift) override {
    PYBIND11_OVERLOAD_PURE(void,                /* Return type */
                           TrajectoryInputFile, /* Parent class */
                           advance,             /* Name of function in C++ (must match Python name) */
                           shift                /* Argument */
    );
  }
};

void populate(pybind11::class_<xmol::trajectory::Trajectory>& pyTrajectory);
void populate(pybind11::class_<xmol::trajectory::TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile);

} // namespace pyxmolpp::v1