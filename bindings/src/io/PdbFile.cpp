#include "PdbFile.h"
#include "xmol/proxy/smart/spans.h"
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<PdbInputFile, xmol::trajectory::TrajectoryInputFile>& pyPdbInputFile) {

  py::enum_<PdbInputFile::Dialect>(pyPdbInputFile, "Dialect", "PDB file dialect")
      .value("STANDARD_V3", PdbInputFile::Dialect::STANDARD_V3,
             "Standard records (https://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html)")
      .value("AMBER_99", PdbInputFile::Dialect::AMBER_99, "Amber-MD convention")
      .export_values();

  pyPdbInputFile
      .def(py::init<std::string, PdbInputFile::Dialect>(), py::arg("filename"),
           py::arg("dialect") = PdbInputFile::Dialect::AMBER_99, "Constructor")
      .def("frames", &PdbInputFile::frames, "Get copy of frames")
      .def("n_frames", &PdbInputFile::n_frames, "Number of frames")
      .def("n_atoms", &PdbInputFile::n_atoms, "Number of atoms in first frame")
      .def("read_frame", &PdbInputFile::read_frame, py::arg("index"), py::arg("frame"),
           "Assign `index` frame coordinates, cell, etc")
      .def("advance", &PdbInputFile::advance, "No-op");
}
