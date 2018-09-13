#include "init.h"

#include "Trajectory.h"

#include <fstream>


using namespace xmol::trajectory;
using namespace xmol::polymer;

void pyxmolpp::trajectory::init_Trajectory(pybind11::module& trajectory) {

  py::class_<TrajectoryPortion, PyTrajectoryPortion>(trajectory, "TrajectoryPortion")
      .def("close", &TrajectoryPortion::close, "Release file handle")
      .def("match", &TrajectoryPortion::match,
           "Checks atom names, residue names and residue ids to match reference_atoms", py::arg("atoms"))
      .def("n_frames", &TrajectoryPortion::n_frames, "Number of frames")
      .def("n_atoms_per_frame", &TrajectoryPortion::n_atoms_per_frame, "Number of atoms per frame")
      .def("set_coordinates",
           (void (TrajectoryPortion::*)(frameIndex_t, const AtomSelection&))(&TrajectoryPortion::set_coordinates),
           py::arg("frame_index"), py::arg("atoms"),
           R"pydoc(Set atoms coordinates from frame `frame_index`)pydoc");

  py::class_<TrajectoryRange>(trajectory,"TrajectoryRange")
      .def("__next__",[](TrajectoryRange& rng) -> Frame&{
        if (rng!=rng){
          Frame& a = *rng;
          ++rng;
          return a;
        }
        throw py::stop_iteration();
      }, py::return_value_policy::reference);

  py::class_<TrajectorySlice>(trajectory, "TrajectorySlice")
      .def("__iter__", &TrajectorySlice::begin)
      .def("__len__", &TrajectorySlice::size);

  py::class_<Trajectory>(trajectory, "Trajectory")
      .def(py::init<const xmol::polymer::Frame&, bool>(), py::arg("reference"),
           py::arg("check_portions_to_match_reference") = true)
      .def("push_trajectory_portion", &Trajectory::push_trajectory_portion, "Add a trajectory portion",
           py::arg("portion"))
      .def("set_update_list", &Trajectory::set_update_list, "Select atoms which will be updated in iterations",
           py::arg("update_list"))
      .def("__len__", &Trajectory::n_frames, "Returns number of frames in trajectory")
      .def_property_readonly("size", &Trajectory::n_frames, "Returns number of frames in trajectory")
      .def_property_readonly("n_frames", &Trajectory::n_frames, "Returns number of frames in trajectory")
      .def("__iter__", &Trajectory::begin)
      .def("__getitem__",
           [](Trajectory& trj, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(static_cast<size_t>(trj.n_frames()), reinterpret_cast<size_t*>(&start),
                                reinterpret_cast<size_t*>(&stop), reinterpret_cast<size_t*>(&step),
                                reinterpret_cast<size_t*>(&slicelength)))
               throw py::error_already_set();
             return trj.slice(start, stop, step);
           })
      .def("__getitem__",
           [](Trajectory& trj, int i) {
             if (i<-trj.n_frames() || i>=trj.n_frames()){
               throw std::out_of_range("Trajectory index is out of bounds");
             }
             if (i<0){
               i+=trj.n_frames();
             }
             Frame frame =*(trj.begin()+i);
             return frame;
           }, py::return_value_policy::move)
      //      .def("__getitem__",&Trajectory::operator[], py::return_value_policy::reference)
      ;
}
