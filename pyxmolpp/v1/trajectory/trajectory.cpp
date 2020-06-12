#include "trajectory.h"

namespace py = pybind11;
using namespace xmol::trajectory;
using namespace xmol;

namespace {

class PyObjectTrajectoryInputFile : public TrajectoryInputFile {
  const py::object m_instance;
  TrajectoryInputFile* ptr;

public:
  PyObjectTrajectoryInputFile(const py::object& instance)
      : m_instance(instance), ptr(&instance.cast<TrajectoryInputFile&>()) {}

  [[nodiscard]] size_t n_frames() const final { return ptr->n_frames(); }
  [[nodiscard]] size_t n_atoms() const final { return ptr->n_atoms(); }

  void read_coordinates(size_t index, xmol::proxy::CoordSpan& coordinates) final {
    ptr->read_coordinates(index, coordinates);
  }

  void advance(size_t shift) override { ptr->advance(shift); }
};
} // namespace

void pyxmolpp::v1::populate(pybind11::class_<Trajectory>& pyTrajectory) {
  //  auto&& pyTrajectoryIterator = py::class_<Trajectory::Iterator>(pyTrajectory, "Iterator");
  auto&& pyTrajectoryFrame = py::class_<Trajectory::Frame, Frame>(pyTrajectory, "Frame");
  auto&& pyTrajectorySlice = py::class_<Trajectory::Slice>(pyTrajectory, "Slice");

  pyTrajectory.def(py::init<Frame>())
      .def("extend", [](Trajectory& self, py::object trajectory_file) {
        if (!py::isinstance<TrajectoryInputFile>(trajectory_file)){
          throw py::type_error("trajectory_file must be derived from TrajectoryInputFile");
        }
        self.extend(PyObjectTrajectoryInputFile(PyObjectTrajectoryInputFile(trajectory_file)));
      }, py::arg("trajectory_file"))
      .def("n_atoms", &Trajectory::n_atoms)
      .def("n_frames", &Trajectory::n_frames)
      .def(
          "__iter__", [](Trajectory& self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

  pyTrajectorySlice.def(
      "__iter__", [](Trajectory::Slice& self) { py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

  pyTrajectoryFrame.def_readonly("index", &Trajectory::Frame::index);
}
void pyxmolpp::v1::populate(py::class_<TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile) {
  pyTrajectoryInputFile.def(py::init<>());
}
