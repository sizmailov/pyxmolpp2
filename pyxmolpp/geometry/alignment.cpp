#include "xmol/geometry/alignment.h"

#include "init.h"
#include "pybind11/eigen.h"
#include "pybind11/stl.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_alignment(pybind11::module& geometry) {


  geometry.def("calc_alignment", xmol::geometry::calc_alignment, py::arg("ref"),
               py::arg("var"),
               R"pydoc(
    Calculate transformation to superimpose var over ref

    Preconditions:
       1) len(ref) == len(var)
       2) len(ref) >= 3
)pydoc");
  geometry.def("calc_geom_center", xmol::geometry::calc_geom_center,
               py::arg("coords"), "Returns mean coordinates");

  geometry.def("calc_mass_center",
      xmol::geometry::calc_mass_center,
      py::arg("coords"),
      py::arg("mass"),
      "Returns center of mass");

  geometry.def("calc_rmsd",
               (double (*)(const std::vector<XYZ>&,
                           const std::vector<XYZ>&))xmol::geometry::calc_rmsd,
               py::arg("ref"), py::arg("var"),
               "Calculate RMSD between ref and var");
  geometry.def("calc_rmsd",
               (double (*)(const std::vector<XYZ>&, const std::vector<XYZ>&,
                           const Transformation3d&))xmol::geometry::calc_rmsd,
               py::arg("ref"), py::arg("var"), py::arg("T"),
               "Calculate RMSD between ref and T*var");

  geometry.def(
      "calc_inertia_tensor",
      (Eigen::Matrix3d (*)(const std::vector<XYZ>&))xmol::geometry::calc_inertia_tensor,
      py::arg("coordinates"),
      "Calculate inertia tensor. All masses are assumed to be 1.0");

  geometry.def(
      "calc_inertia_tensor",
      (Eigen::Matrix3d (*)(const std::vector<XYZ>&, const std::vector<double>&))xmol::geometry::calc_inertia_tensor,
      py::arg("coordinates"), py::arg("mass"),
      "Calculate inertia tensor");
}
