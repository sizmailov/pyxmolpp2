#include "AmberNetCDF.h"
#include "xmol/proxy/smart/spans.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<AmberNetCDF, xmol::trajectory::TrajectoryInputFile>& pyAmberNetCDF) {

  pyAmberNetCDF.def(py::init<std::string>(), py::arg("filename"), "Amber binary trajectory ``.nc`` file")
      .def("n_frames", &AmberNetCDF::n_frames, "Number of frames")
      .def("n_atoms", &AmberNetCDF::n_atoms, "Number of atoms per frame")
      .def(
          "read_coordinates",
          [](AmberNetCDF& self, size_t index, CoordSmartSpan& span) { self.read_coordinates(index, span); },
          py::arg("index"), py::arg("coords"), "Assign `index` frame coordinates to `coords`")
      .def("advance", &AmberNetCDF::advance, py::arg("shift"), "Shift internal pointer by `shift`");
  ;
}
