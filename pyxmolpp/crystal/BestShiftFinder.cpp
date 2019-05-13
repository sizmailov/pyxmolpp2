#include "init.h"

#include "xmol/crystal/LatticeVectors.h"

using namespace xmol::crystal;
using namespace xmol::geometry;

void pyxmolpp::crystal::init_best_shift_finder(py::module& crystal) {
  py::class_<BestShiftFinder>(crystal, "BestShiftFinder")
      .def(py::init<LatticeVectors>(),
           py::arg("lattice_vectors"))
      .def("find_best_shift",
           &BestShiftFinder::find_best_shift, py::arg("ref"), py::arg("var"),
           R"pydoc(
:param ref: reference point
:param var: variable point
:returns: `shift` that minimizes :math:`(ref-(var+shift))^2`
)pydoc")
      .def("scale_lattice_by",
           &BestShiftFinder::scale_lattice_by,
           py::arg("factor"),
           "Uniformly scale lattice vectors"
      );
}
