#include "init.h"

#include "xmol/geometry/calculate_sasa.h"

#include "pybind11/operators.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

using namespace xmol::geometry;

void pyxmolpp::geometry::init_calc_sasa(pybind11::module& geometry) {

  auto calc_sasa_lambda = [](
      std::vector<XYZ>& coords,
      py::buffer& radii,
      double solvent_radius,
      py::array* indices_of_interest,
      int n_samples
  ){
        py::buffer_info radii_info = radii.request();

        /* Some sanity checks ... */
        if (radii_info.ndim != 1) {
          throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: radii dimension != 1");
        }

        if (radii_info.format != "d") {
          throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: radii dtype  != double");
        }

        if (radii_info.size != coords.size()) {
          throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: radii.size() != coords.size()");
        }

        auto radii_begin = static_cast<double* >(radii_info.ptr);
        auto radii_end = static_cast<double* >(radii_info.ptr)+radii_info.size;


        int* indices_of_interest_begin = nullptr;
        int* indices_of_interest_end = nullptr;

        if (indices_of_interest!=nullptr){

          py::buffer_info indices_of_interest_info = indices_of_interest->request();

          if (indices_of_interest_info.ndim != 1) {
            throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: indices_of_interest dimension != 1");
          }

          if (indices_of_interest_info.format != "i") {
            throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: indices_of_interest dtype("+indices_of_interest_info.format +") != numpy.intc");
          }

          if (indices_of_interest_info.size > coords.size()) {
            throw std::runtime_error("pyxmolpp2.geometry.calc_sasa: indices_of_interest.size() > coords.size()");
          }

          indices_of_interest_begin =  static_cast<int* >(indices_of_interest_info.ptr);
          indices_of_interest_end =  static_cast<int* >(indices_of_interest_info.ptr)+indices_of_interest_info.size;

        }


        auto result = new std::vector<double>(calculate_sasa(
            coords,
            radii_begin, radii_end,
            solvent_radius,
            n_samples,
            indices_of_interest_begin, indices_of_interest_end)
        );
        auto cap = py::capsule(result, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
        return py::array(result->size(), result->data(), cap);
  };

  geometry.def("calc_sasa",
               calc_sasa_lambda,
               py::arg("coordinates"),
               py::arg("vdw_radii"),
               py::arg("solvent_radius"),
               py::arg("indices_of_interest").noconvert(true),
               py::arg("n_samples").noconvert(true)=20,
               R"pydoc(Calculates per atom Solvent Accessible Solvent Area (SASA) using Lee & Richards' algorithm

:param coordinates: list of atom coordinates
:param vdw_radii: list of atom VDW radii
:param solvent_radii: radius of solvent (1.4 is common value for water)
:param n_samples: number of atom slices (default=20). linearly increases complexity
:param indices_of_interest: limit calculation of SASA to points with given indices. If `None` calculate for all points
:returns: List of per-atom exposed area (0 for fully buried atoms, :math:`4\pi R_{vdw}^2` for isolated atoms)
)pydoc");

  geometry.def("calc_sasa",
      [&calc_sasa_lambda](
          std::vector<XYZ>& coords,
          py::buffer& radii,
          double solvent_radius,
          int n_samples){
        return calc_sasa_lambda(coords, radii, solvent_radius, nullptr, n_samples);
      },
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
