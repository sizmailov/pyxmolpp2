#pragma once

#include "init.h"

#include "xmol/trajectory/Trajectory.h"

namespace pyxmolpp {
namespace trajectory {


class PyTrajectoryPortion : public xmol::trajectory::TrajectoryPortion {
public:
  /* Inherit the constructors */
  using TrajectoryPortion::TrajectoryPortion;

  void set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                       const xmol::polymer::AtomSelection& atoms, const std::vector<int>& /*update_list*/) override {
    PYBIND11_OVERLOAD_PURE(
        void,              /* Return type */
        TrajectoryPortion, /* Parent class */
        set_coordinates,   /* Name of function in C++ (must
                                                   match Python name) */
        frameIndex,        /* Argument(s) */
        atoms);
  }
  xmol::polymer::frameIndex_t n_frames() const override {
    PYBIND11_OVERLOAD_PURE(
        xmol::polymer::frameIndex_t, /* Return type */
        TrajectoryPortion,           /* Parent class */
        n_frames, /* Name of function in C++ (must match Python name) */
        );
  }
  xmol::polymer::atomIndex_t n_atoms_per_frame() const override {
    PYBIND11_OVERLOAD_PURE(
        xmol::polymer::atomIndex_t, /* Return type */
        TrajectoryPortion,          /* Parent class */
        n_atoms_per_frame,           /* Name of function in C++ (must
                                                              match Python name) */
        );
  }
  bool match(const xmol::polymer::AtomSelection& atoms) const override {
    PYBIND11_OVERLOAD_PURE(
        bool,              /* Return type */
        TrajectoryPortion, /* Parent class */
        match, /* Name of function in C++ (must match Python name) */
        atoms  /* Argument(s) */
        );
  }
  void close() override {
    PYBIND11_OVERLOAD_PURE(
        void,              /* Return type */
        TrajectoryPortion, /* Parent class */
        close, /* Name of function in C++ (must match Python name) */
        );
  }
};
}
}
