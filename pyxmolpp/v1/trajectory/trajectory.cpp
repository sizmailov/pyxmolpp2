#include "trajectory.h"

namespace py = pybind11;
using namespace xmol::trajectory;
using namespace xmol;

void pyxmolpp::v1::populate(pybind11::class_<Trajectory>& pyTrajectory) {
  //  auto&& pyTrajectoryIterator = py::class_<Trajectory::Iterator>(pyTrajectory, "Iterator");
  auto&& pyTrajectoryFrame = py::class_<Trajectory::Frame, Frame>(pyTrajectory, "Frame");
  auto&& pyTrajectorySlice = py::class_<Trajectory::Slice>(pyTrajectory, "Slice");

  pyTrajectory.def(py::init<Frame>())
      .def("extend", [](Trajectory& self, PyTrajectoryInputFile file) { self.extend(std::move(file)); })
      .def("n_atoms", &Trajectory::n_atoms)
      .def("n_frames", &Trajectory::n_frames)
      .def(
          "__iter__", [](Trajectory& self) { py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

  pyTrajectorySlice.def(
      "__iter__", [](Trajectory::Slice& self) { py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

  pyTrajectoryFrame.def_readonly("index", &Trajectory::Frame::index);
}
void pyxmolpp::v1::populate(py::class_<TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile) {
  pyTrajectoryInputFile.def(py::init<>());
}
