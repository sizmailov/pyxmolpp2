#include "PdbFile.h"
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace xmol::io;

void pyxmolpp::v1::populate(py::class_<PdbInputFile, xmol::trajectory::TrajectoryInputFile>& pyPdbInputFile) {
  py::enum_<PdbInputFile::Dialect>(pyPdbInputFile, "Dialect")
      .value("AMBER_99",PdbInputFile::Dialect::AMBER_99)
      .value("STANDARD_V3",PdbInputFile::Dialect::STANDARD_V3)
      .export_values()
  ;
  pyPdbInputFile
      .def(py::init<std::string, PdbInputFile::Dialect>(), py::arg("self"), py::arg("dialect")=PdbInputFile::Dialect::AMBER_99)
      .def("frames", &PdbInputFile::frames)
  ;
}
