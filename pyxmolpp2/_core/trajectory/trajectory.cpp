#include "trajectory.h"
#include "iterator-helpers.h"

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
  auto&& pyTrajectoryIterator = py::class_<Trajectory::Iterator>(pyTrajectory, "Iterator");
  auto&& pyTrajectoryFrame = py::class_<Trajectory::Frame, Frame>(pyTrajectory, "Frame");
  auto&& pyTrajectorySlice = py::class_<Trajectory::Slice>(pyTrajectory, "Slice");

  pyTrajectory.def(py::init<Frame>())
      .def(
          "extend",
          [](Trajectory& self, py::object trajectory_file) {
            if (!py::isinstance<TrajectoryInputFile>(trajectory_file)) {
              throw py::type_error("trajectory_file must be derived from TrajectoryInputFile");
            }
            self.extend(PyObjectTrajectoryInputFile(PyObjectTrajectoryInputFile(trajectory_file)));
          },
          py::arg("trajectory_file"))
      .def_property_readonly("n_atoms", &Trajectory::n_atoms)
      .def_property_readonly("n_frames", &Trajectory::n_frames)
      .def_property_readonly("size", &Trajectory::n_frames)
      .def("__len__", &Trajectory::n_frames)
      .def("__getitem__",
           [](Trajectory& trj, int idx) -> Trajectory::Frame {
             int i = idx;
             if (i < 0) {
               i += trj.n_frames();
             }
             if (i < 0 || i >= trj.n_frames()) {
               throw py::index_error("Bad trajectory index " + std::to_string(idx) + "");
             }
             return trj.at(i);
           })
      .def("__getitem__",
           [](Trajectory& trj, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(trj.n_frames(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start>=0);
             assert(stop>=0);
             return trj.slice(start, stop, step);
           })
      .def(
          "__iter__", [](Trajectory& self) { return common::make_iterator(self.begin(), self.end()); },
          py::keep_alive<0, 1>());

  pyTrajectorySlice.def(
      "__iter__", [](Trajectory::Slice& self) { return common::make_iterator(self.begin(), self.end()); },
      py::keep_alive<0, 1>())
      .def("__len__", &Trajectory::Slice::size)
      .def("__getitem__",
           [](Trajectory::Slice& self, int idx) -> Trajectory::Frame {
             int i = idx;
             if (i < 0) {
               i += self.size();
             }
             if (i < 0 || i >= self.size()) {
               throw py::index_error("Bad trajectory slice index " + std::to_string(idx) + "");
             }
             return self.at(i);
           });

  pyTrajectoryFrame.def_readonly("index", &Trajectory::Frame::index);
}
void pyxmolpp::v1::populate(py::class_<TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile) {
  pyTrajectoryInputFile.def(py::init<>());
}
