#include "init.h"

#include <pybind11/operators.h>
#include <pybind11/stl_bind.h>

#include "xmol/geometry/Transformation3d.h"


using namespace xmol::geometry;

void pyxmolpp::geometry::init_VectorXYZ(pybind11::module& geometry) {

  using VectorXYZ = std::vector<xmol::geometry::XYZ>;
  auto pyVectorXYZ = py::bind_vector<VectorXYZ>(geometry, "VectorXYZ");

  pyVectorXYZ
      .def(
          "transform",
          [](VectorXYZ& sel, xmol::geometry::UniformScale3d& transformation3d) {
            for (auto&r :sel){
              r = transformation3d.transform(r);
            }
            return sel;
          },
          py::arg("uniformScale3d"),
          "Applies transformation inplace")

      .def(
          "transform",
          [](VectorXYZ& sel, xmol::geometry::Translation3d& transformation3d) {
            for (auto&r :sel){
              r = transformation3d.transform(r);
            }
            return sel;
          },
          py::arg("translation3d"),
          "Applies transformation inplace")

      .def(
          "transform",
          [](VectorXYZ& sel, xmol::geometry::Rotation3d& transformation3d) {
            for (auto&r :sel){
              r = transformation3d.transform(r);
            }
            return sel;
          },
          py::arg("rotation3d"),
          "Applies transformation inplace")

      .def(
          "transform",
          [](VectorXYZ& sel, xmol::geometry::Transformation3d& transformation3d) {
            for (auto&r :sel){
              r = transformation3d.transform(r);
            }
            return sel;
          },
          py::arg("transformation3d"),
          "Applies transformation inplace")
      ;
}
