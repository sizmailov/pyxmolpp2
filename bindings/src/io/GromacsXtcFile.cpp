#include "GromacsXtcFile.h"
#include "xmol/Frame.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<GromacsXtcFile, xmol::trajectory::TrajectoryInputFile>& pyGromacsXtc) {

  pyGromacsXtc.def(py::init<std::string, size_t>(), py::arg("filename"), py::arg("n_frames"))
      .def("n_frames", &GromacsXtcFile::n_frames, "Number of frames")
      .def("n_atoms", &GromacsXtcFile::n_atoms, "Number of atoms per frame")
      .def("read_frame", &GromacsXtcFile::read_frame, py::arg("index"), py::arg("frame"),
           "Assign `index` frame coordinates, cell, etc")
      .def("advance", &GromacsXtcFile::advance, py::arg("shift"), "Shift internal pointer by `shift`");
  ;
}

void pyxmolpp::v1::populate(pybind11::class_<xmol::io::xdr::XtcWriter>& pyXtcWriter) {

  pyXtcWriter
      .def(py::init<std::string, float>(), py::arg("filename"), py::arg("precision"))
      .def("write", &xdr::XtcWriter::write, "Write frame");
}
