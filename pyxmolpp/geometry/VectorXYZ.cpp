#include "init.h"

#include <pybind11/operators.h>
#include <pybind11/stl_bind.h>
#include <pybind11/eigen.h>

#include "xmol/geometry/Transformation3d.h"


using namespace xmol::geometry;

void pyxmolpp::geometry::init_VectorXYZ(pybind11::module& geometry) {

  using VectorXYZ = std::vector<xmol::geometry::XYZ>;
  auto pyVectorXYZ = py::bind_vector<VectorXYZ>(geometry, "VectorXYZ");

  pyVectorXYZ
      .def_static("from_numpy",
          [](Eigen::MatrixX3d& array) {
            VectorXYZ vectorXYZ;
            vectorXYZ.reserve(array.rows());
            for (int i=0;i<array.rows();i++){
              vectorXYZ.push_back(XYZ(array(i,0),array(i,1),array(i,2)));
            }
            return vectorXYZ;
          },
          py::arg("ndarray"),
          "Constructs VectorXYZ from numpy array of shape (n,3)")

      .def("to_numpy",
          [](VectorXYZ& array) {
            Eigen::MatrixX3d result;
            result.resize(array.size(), 3);

            for (int i=0;i<array.size();i++){
              const auto& r = array[i];
              result(i,0) = r.x();
              result(i,1) = r.y();
              result(i,2) = r.z();
            }
            return result;
          },
          "Convert to numpy array of shape (n,3)")

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
