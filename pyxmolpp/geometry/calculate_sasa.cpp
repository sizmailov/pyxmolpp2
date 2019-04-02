#include "init.h"

#include "xmol/geometry/calculate_sasa.h"

#include "pybind11/operators.h"
#include "pybind11/stl.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_calc_sasa(pybind11::module& geometry) {

  geometry.def("calc_sasa",
      calculate_sasa,
      py::arg("coordinates"),
      py::arg("vdw_radii"),
      py::arg("solvent_radius"),
      py::arg("n_samples")=20,
      R"pydoc(Calculates per atom Solvent Accessible Solvent Area (SASA) using Lee & Richards' algorithm

:param coordinates: list of atom coordinates
:param vdw_radii: list of atom VDW radii
:param solvent_radii: radius of solvent (1.4 is common value for water)
:param n_samples: number of atom slices (default=20). linearly increases complexity
:returns: List of per-atom exposed area (0 for fully buried atoms, :math:`4\pi R_{vdw}^2` for isolated atoms)
)pydoc");
}
