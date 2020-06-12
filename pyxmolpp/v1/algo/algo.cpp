#include "algo.h"
#include "xmol/algo/alignment-impl.h"
#include "xmol/algo/alignment.h"
#include "xmol/algo/sasa.h"
#include "xmol/algo/vector-correlation.h"
#include "xmol/base.h"

#include <iostream>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
using namespace xmol;
namespace py = pybind11;

void pyxmolpp::v1::define_algo_functions(pybind11::module& m) {
  m.def(
      "calc_alignment",
      [](xmol::CoordEigenMatrix& reference, xmol::CoordEigenMatrix& variable) {
        return algo::calc_alignment_impl(reference, variable);
      },
      py::arg("ref"), py::arg("var"));
  m.def(
      "calc_rmsd",
      [](xmol::CoordEigenMatrix& reference, xmol::CoordEigenMatrix& variable) {
        return algo::calc_rmsd_impl(reference, variable);
      },
      py::arg("ref"), py::arg("var"));
  m.def(
      "calc_inertia_tensor", [](xmol::CoordEigenMatrix& coords) { return algo::calc_inertia_tensor_impl(coords); },
      py::arg("coords"));
  m.def(
      "calc_autocorr_order_2",
      [](py::array_t<double, py::array::f_style>& array, int limit) {
        if (array.ndim() != 2 || array.shape(0) != 3) {
          throw py::type_error("shape!=(3,*)");
        }
        int N = array.shape(1);
        if (limit < 0 || limit > N) {
          limit = N;
        }
        py::array_t<double> result(limit);
        future::Span<XYZ> xyz_span(reinterpret_cast<XYZ*>(array.mutable_data()), array.size());
        algo::calc_autocorr_order_2(xyz_span, future::Span(result.mutable_data(), result.size()),
                                    algo::AutoCorrelationMode::NORMALIZE_VECTORS);
        return result;
      },
      py::arg("vectors"), py::arg("limit") = -1);

  m.def(
      "calc_autocorr_order_2_PRE",
      [](py::array_t<double, py::array::f_style>& array, int limit) {
        if (array.ndim() != 2 || array.shape(0) != 3) {
          throw py::type_error("shape!=(3,*)");
        }
        int N = array.shape(1);
        if (limit < 0 || limit > N) {
          limit = N;
        }
        py::array_t<double> result(limit);
        future::Span<XYZ> xyz_span(reinterpret_cast<XYZ*>(array.mutable_data()), array.size());
        algo::calc_autocorr_order_2(xyz_span, future::Span(result.mutable_data(), result.size()),
                                    algo::AutoCorrelationMode::NORMALIZE_AND_DIVIDE_BY_CUBE);
        return result;
      },
      py::arg("vectors"), py::arg("limit") = -1);
  m.def(
      "calc_sasa",
      [](py::array_t<double, py::array::f_style> coords, py::array_t<double> coord_radii, double solvent_radii,
         int n_samples, py::array_t<int>* indices_of_interest) {
        if (coords.ndim() != 2 || coords.shape(0) != 3) {
          throw py::type_error("coords.shape!=(3,*)");
        }
        future::Span<int> indices;
        if (indices_of_interest != nullptr) {
          py::buffer_info indices_of_interest_info = indices_of_interest->request();
          if (indices_of_interest_info.ndim != 1) {
            throw py::type_error("indices_of_interest dimension != 1");
          }
          if (indices_of_interest_info.format != "i") {
            throw py::type_error("indices_of_interest dtype(" + indices_of_interest_info.format + ") != numpy.intc");
          }
          if (indices_of_interest_info.size > coords.size()) {
            throw py::type_error("indices_of_interest.size() > coords.size()");
          }
          indices = future::Span{indices_of_interest->mutable_data(), (size_t)indices_of_interest->size()};
        }
        future::Span<XYZ> coord_span(reinterpret_cast<XYZ*>(coords.mutable_data()), coords.size());
        const int limit = indices_of_interest ? indices_of_interest->size() : coords.size();
        py::array_t<double> result(limit);
        future::Span<double> result_span(result.mutable_data(), (size_t)result.size());
        future::Span<double> coord_radii_span(coord_radii.mutable_data(), (size_t)coord_radii.size());
        algo::calc_sasa(coord_span, coord_radii_span, solvent_radii, result_span, n_samples, indices);
        return result;
      },
      py::arg("coordinates"), py::arg("vdw_radii"), py::arg("solvent_radius"),
      py::arg("indices_of_interest").noconvert(true) = nullptr, py::arg("n_samples").noconvert(true) = 20);
}
