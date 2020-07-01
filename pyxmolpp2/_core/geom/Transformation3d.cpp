#include "Transformation3d.h"

#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

using namespace xmol::geom::affine;
using namespace xmol::geom;

namespace py = pybind11;

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::affine::Rotation3d>& pyRotation) {
  pyRotation.def(py::init<>(), "Default constructor")
      .def(py::init<XYZ, AngleValue>(), py::arg("rotation_axis"), py::arg("rotation_angle"), "Construct from axis and angle")
      .def(py::init<const Eigen::Matrix3d>(), py::arg("rotation_matrix"), "Construct from matrix")
      .def("transform", &Rotation3d::transform, py::arg("r"), "Returns rotated point")
      .def("inverted", &Rotation3d::inverted, "Inverted rotation")
      .def("axis", &Rotation3d::axis, "Rotational axis")
      .def("theta", &Rotation3d::theta, "Rotational angle")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(UniformScale3d() * py::self)
      .def(
          "matrix3d",
          [](Rotation3d& rotation3d) {
            py::array_t<double, py::array::f_style> result({3, 3});
            auto matrix = rotation3d.get_underlying_matrix();
            std::copy(matrix.data(), matrix.data() + 9, result.mutable_data());
            return result;
          },
          "Rotational matrix");
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::affine::Transformation3d>& pyTransformation) {
  pyTransformation.def(py::init<>())
      .def(py::init<Rotation3d, Translation3d>(), py::arg("rotation_followed_by"), py::arg("translation"))
      .def("transform", &Transformation3d::transform, py::arg("r"), "Returns transformed point")
      .def("inverted", &Transformation3d::inverted, "Inverted transformation")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * Rotation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(Rotation3d() * py::self)
      .def(UniformScale3d() * py::self)
      .def(
          "matrix3d",
          [](Transformation3d& transformation3d) {
            py::array_t<double, py::array::f_style> result({3, 3});
            auto matrix = transformation3d.get_underlying_matrix();
            std::copy(matrix.data(), matrix.data() + 9, result.mutable_data());
            return result;
          },
          "Non-translational part of transformation")
      .def(
          "vector3d", [](Transformation3d& transformation3d) { return transformation3d.get_translation(); },
          "Translational part of transformation");
}

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::affine::Translation3d>& pyTranslation) {
  pyTranslation.def(py::init<>(), "Identity transformation")
      .def(py::init<XYZ>(), py::arg("dr"), ":param a: translation vector")
      .def("transform", &Translation3d::transform, py::arg("r"), "Returns translated point")
      .def("inverted", &Translation3d::inverted, "Inverted transform")
      .def("dr", &Translation3d::dr, "Translation vector")
      .def(py::self * py::self);
}

void pyxmolpp::v1::populate(pybind11::class_<xmol::geom::affine::UniformScale3d>& pyUniformScale) {
  pyUniformScale.def(py::init<>())
      .def(py::init<double>(), py::arg("scale_factor"))
      .def("transform", &UniformScale3d::transform, py::arg("r"), "Returns scaled point")
      .def("inverted", &UniformScale3d::inverted, "Inverted transform")
      .def_property_readonly("scale", &UniformScale3d::scale, "Linear scale factor")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(Translation3d() * py::self);
}
