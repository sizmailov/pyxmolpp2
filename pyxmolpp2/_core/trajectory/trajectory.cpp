#include "trajectory.h"
#include "iterator-helpers.h"
#include "xmol/proxy/smart/CoordSmartSpan.h"

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

  void read_frame(size_t index, Frame& frame) final { ptr->read_frame(index, frame); }
  void advance(size_t shift) final { ptr->advance(shift); }
};
} // namespace

void pyxmolpp::v1::populate(pybind11::class_<Trajectory>& pyTrajectory) {
  auto&& pyTrajectoryIterator = py::class_<Trajectory::Iterator>(pyTrajectory, "Iterator");
  auto&& pyTrajectorySlice = py::class_<Trajectory::Slice>(pyTrajectory, "Slice");

  pyTrajectory.def(py::init<Frame>())
      .def(
          "extend",
          [](Trajectory& self, py::object& trajectory_file) {
            if (!py::isinstance<TrajectoryInputFile>(trajectory_file)) {
              throw py::type_error("trajectory_file must be derived from TrajectoryInputFile");
            }
            self.extend(PyObjectTrajectoryInputFile(trajectory_file));
          },
          py::arg("trajectory_file"), "Extend trajectory")
      .def_property_readonly("n_atoms", &Trajectory::n_atoms, "Number of atoms in frame")
      .def_property_readonly("n_frames", &Trajectory::n_frames, "Number of frames")
      .def_property_readonly("size", &Trajectory::n_frames, "Number of frames")
      .def("__len__", &Trajectory::n_frames)
      .def("__getitem__",
           [](Trajectory& trj, int idx) -> Frame {
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
      .def_property_readonly("n_atoms", &Trajectory::Slice::n_atoms, "Number of atoms in frame")
      .def_property_readonly("n_frames", &Trajectory::Slice::n_frames, "Number of frames")
      .def("__getitem__",
           [](Trajectory::Slice& self, int idx) -> Frame {
             int i = idx;
             if (i < 0) {
               i += self.size();
             }
             if (i < 0 || i >= self.size()) {
               throw py::index_error("Bad trajectory slice index " + std::to_string(idx) + "");
             }
             return self.at(i);
           });
}

void pyxmolpp::v1::populate(py::class_<TrajectoryInputFile, PyTrajectoryInputFile>& pyTrajectoryInputFile) {
  pyTrajectoryInputFile.def(py::init<>())
      .def("n_frames", &TrajectoryInputFile::n_frames, "Number of frames")
      .def("n_atoms", &TrajectoryInputFile::n_atoms, "Number of atoms per frame")
      .def("read_frame", &TrajectoryInputFile::read_frame, py::arg("index"), py::arg("frame"),
           "Assign `index` frame coordinates, cell, etc")
      .def("advance", &TrajectoryInputFile::advance, "Shift internal data pointer", py::arg("shift"));
}

void pyxmolpp::v1::PyTrajectoryInputFile::read_frame(size_t index, Frame& frame) {
  py::object pyFrame = py::cast(frame, py::return_value_policy::reference);
  PYBIND11_OVERLOAD_PURE(void,                /* Return type */
                         TrajectoryInputFile, /* Parent class */
                         read_frame,          /* Name of function in C++ (must match Python name) */
                         index, pyFrame       /* Arguments */
  );
}

size_t pyxmolpp::v1::PyTrajectoryInputFile::n_frames() const {
  PYBIND11_OVERLOAD_PURE(size_t,              /* Return type */
                         TrajectoryInputFile, /* Parent class */
                         n_frames             /* Name of function in C++ (must match Python name) */
  );
}

size_t pyxmolpp::v1::PyTrajectoryInputFile::n_atoms() const {
  PYBIND11_OVERLOAD_PURE(size_t,              /* Return type */
                         TrajectoryInputFile, /* Parent class */
                         n_atoms              /* Name of function in C++ (must match Python name) */
  );
}

void pyxmolpp::v1::PyTrajectoryInputFile::advance(size_t shift) {
  PYBIND11_OVERLOAD_PURE(void,                /* Return type */
                         TrajectoryInputFile, /* Parent class */
                         advance,             /* Name of function in C++ (must match Python name) */
                         shift                /* Argument */
  );
}
