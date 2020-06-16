#include "PdbFile.h"
#include "xmol/proxy/smart/spans.h"
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<PdbInputFile, xmol::trajectory::TrajectoryInputFile>& pyPdbInputFile) {

  py::enum_<PdbInputFile::Dialect>(pyPdbInputFile, "Dialect")
      .value("AMBER_99", PdbInputFile::Dialect::AMBER_99)
      .value("STANDARD_V3", PdbInputFile::Dialect::STANDARD_V3)
      .export_values();

  pyPdbInputFile
      .def(py::init<std::string, PdbInputFile::Dialect>(), py::arg("filename"),
           py::arg("dialect") = PdbInputFile::Dialect::AMBER_99)
      .def("frames", &PdbInputFile::frames)
      .def("n_frames", &PdbInputFile::n_frames)
      .def("n_atoms", &PdbInputFile::n_atoms)
      .def("read_coordinates",
           [](PdbInputFile& self, size_t index, CoordSmartSpan& span) { self.read_coordinates(index, span); })
      .def("advance", &PdbInputFile::advance);
}
