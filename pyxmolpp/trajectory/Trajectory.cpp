#include "init.h"

#include "Trajectory.h"

#include <fstream>


using namespace xmol::trajectory;
using namespace xmol::polymer;

void pyxmolpp::trajectory::init_Trajectory(pybind11::module& trajectory) {


  py::class_<TrajectoryPortion,PyTrajectoryPortion>(trajectory, "TrajectoryPortion")
      .def("close",&TrajectoryPortion::close)
      .def("match",&TrajectoryPortion::match)
      .def("n_frames",&TrajectoryPortion::n_frames)
      .def("n_atoms_per_frame",&TrajectoryPortion::n_atoms_per_frame)
      .def("set_coordinates",(void (TrajectoryPortion::*)(frameIndex_t, const AtomSelection&))(&TrajectoryPortion::set_coordinates))
      ;
}
