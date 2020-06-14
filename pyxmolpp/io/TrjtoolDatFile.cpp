#include "TrjtoolDatFile.h"
#include "xmol/proxy/smart/spans.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<TrjtoolDatFile, xmol::trajectory::TrajectoryInputFile>& pyTrjtoolDatFile) {

  pyTrjtoolDatFile.def(py::init<std::string>(), py::arg("filename"))
      .def("n_frames", &TrjtoolDatFile::n_frames)
      .def("n_atoms", &TrjtoolDatFile::n_atoms)
      .def("read_coordinates",
           [](TrjtoolDatFile& self, size_t index, CoordSmartSpan& span) { self.read_coordinates(index, span); })
      .def("advance", &TrjtoolDatFile::advance);
  ;
}
