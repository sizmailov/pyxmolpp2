#include "TrjtoolDatFile.h"
#include "xmol/Frame.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<TrjtoolDatFile, xmol::trajectory::TrajectoryInputFile>& pyTrjtoolDatFile) {

  pyTrjtoolDatFile.def(py::init<std::string>(), py::arg("filename"))
      .def("n_frames", &TrjtoolDatFile::n_frames, "Number of frames")
      .def("n_atoms", &TrjtoolDatFile::n_atoms, "Number of atoms per frame")
      .def("read_frame", &TrjtoolDatFile::read_frame, py::arg("index"), py::arg("frame"),
           "Assign `index` frame coordinates, cell, etc")
      .def("advance", &TrjtoolDatFile::advance, py::arg("shift"), "Shift internal pointer by `shift`");
  ;
}
