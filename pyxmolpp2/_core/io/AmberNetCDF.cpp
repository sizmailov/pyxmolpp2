#include "AmberNetCDF.h"
#include "xmol/proxy/smart/spans.h"
#include "xmol/geom/UnitCell.h"

namespace py = pybind11;
using namespace xmol::io;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(py::class_<AmberNetCDF, xmol::trajectory::TrajectoryInputFile>& pyAmberNetCDF) {

  pyAmberNetCDF.def(py::init<std::string>(), py::arg("filename"), "Amber binary trajectory ``.nc`` file")
      .def("n_frames", &AmberNetCDF::n_frames, "Number of frames")
      .def("n_atoms", &AmberNetCDF::n_atoms, "Number of atoms per frame")
      .def(
          "read_frame",
          [](AmberNetCDF& self, size_t index, CoordSmartSpan& span, xmol::geom::UnitCell& cell) { self.read_frame(index, span, cell); },
          py::arg("index"), py::arg("coords"), py::arg("cell"), "Assign `index` frame coordinates to `coords`")
      .def("advance", &AmberNetCDF::advance, py::arg("shift"), "Shift internal pointer by `shift`");
  ;
}
