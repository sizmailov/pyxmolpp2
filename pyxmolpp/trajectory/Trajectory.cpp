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

  py::class_<TrajectoryRange>(trajectory,"TrajectoryRange")
      .def("__next__",[](TrajectoryRange& rng) -> Frame&{
        if (rng!=rng){
          Frame& a = *rng;
          ++rng;
          return a;
        }
        throw py::stop_iteration();
      }, py::return_value_policy::reference);

  py::class_<TrajectorySlice>(trajectory,"TrajectorySlice")
      .def("__iter__", &TrajectorySlice::begin);

  py::class_<Trajectory>(trajectory, "Trajectory")
      .def(py::init<const xmol::polymer::Frame&, bool>())
      .def("push_trajectory_portion", &Trajectory::push_trajectory_portion)
      .def("__len__", &Trajectory::n_frames)
      .def_property_readonly("size", &Trajectory::n_frames)
      .def_property_readonly("n_frames", &Trajectory::n_frames)
      .def("__iter__", &Trajectory::begin)
      .def("__getitem__",[](Trajectory& trj, py::slice& slice){
        size_t start, stop, step, slicelength;
        if (!slice.compute(trj.n_frames(), &start, &stop, &step, &slicelength))
          throw py::error_already_set();
        return trj.slice(start,stop,step);
      })
//      .def("__getitem__",&Trajectory::operator[], py::return_value_policy::reference)
      ;
}
