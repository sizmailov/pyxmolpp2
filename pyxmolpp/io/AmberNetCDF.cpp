#include "AmberNetCDF.h"
#include "xmol/proxy/smart/spans.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<AmberNetCDF, xmol::trajectory::TrajectoryInputFile>& pyAmberNetCDF) {

  pyAmberNetCDF.def(py::init<std::string>(), py::arg("filename"))
      .def("n_frames", &AmberNetCDF::n_frames)
      .def("n_atoms", &AmberNetCDF::n_atoms)
      .def("read_coordinates",
           [](AmberNetCDF& self, size_t index, CoordSmartSpan& span) { self.read_coordinates(index, span); })
      .def("advance", &AmberNetCDF::advance);
  ;
}
