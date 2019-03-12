#include "init.h"

#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>

#include "xmol/geometry/Transformation3d.h"


using namespace xmol::geometry;

void pyxmolpp::geometry::init_Transformation3d(pybind11::module& geometry) {

  auto&& pyTranslation3d = py::class_<Translation3d>(geometry,"Translation3d",R"pydoc(Represents translation by 3d vector)pydoc");
  auto&& pyUniformScale3d = py::class_<UniformScale3d>(geometry,"UniformScale3d",R"pydoc(Represents uniform 3d scaling)pydoc");
  auto&& pyRotation3d = py::class_<Rotation3d>(geometry,"Rotation3d",R"pydoc(Represents 3d rotation)pydoc");
  auto&& pyTransformation3d = py::class_<Transformation3d>(geometry,"Transformation3d",
      R"pydoc(
Represents arbitrary 3d transformation. The result of mixing two of :py:class:`Translation3d`, :py:class:`UniformScale3d`, :py:class:`Rotation3d` or :py:class:`Transformation3d`
)pydoc"
      );

  pyTranslation3d
      .def(py::init<>(), "Identity transformation")
      .def(py::init<XYZ>(), py::arg("dr"), ":param a: translation vector")
      .def("transform",&Translation3d::transform, py::arg("r"),"Returns translated point")
      .def("inverted",&Translation3d::inverted)
      .def("dr",&Translation3d::dr)
      .def(py::self * py::self)
      ;

  pyUniformScale3d
      .def(py::init<>())
      .def(py::init<double>(),py::arg("scale_factor"))
      .def("transform",&UniformScale3d::transform, py::arg("r"), "Returns scaled point")
      .def("inverted",&UniformScale3d::inverted, "Returns inverted scale transformation")
      .def_property_readonly("scale",&UniformScale3d::scale,"Returns scale factor as :py:class:`float`")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(Translation3d() * py::self)
  ;

  pyRotation3d
      .def(py::init<>())
      .def(py::init<XYZ,AngleValue>(),py::arg("rotation_axis"),py::arg("rotation_angle"))
      .def(py::init<const Eigen::Matrix3d>(), py::arg("rotation_matrix"))
      .def("transform",&Rotation3d::transform, py::arg("r"),"Returns rotated point")
      .def("inverted",&Rotation3d::inverted, "Returns inverted rotation")
      .def("axis",&Rotation3d::axis, "Returns axis of rotation")
      .def("theta",&Rotation3d::theta, "Returns angle of rotation")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(UniformScale3d() * py::self)
      .def("matrix3d",[](Rotation3d& rotation3d){
        py::array_t<double, py::array::f_style> result({3,3});
        auto matrix =rotation3d.get_underlying_matrix();
        std::copy(matrix.data(),matrix.data()+9, result.mutable_data());
        return result;
        },
        "Returns underlying rotational matrix"
        );

  pyTransformation3d.def(py::init<>())
      .def(py::init<Rotation3d, Translation3d>(), py::arg("rotation_followed_by"), py::arg("translation"))
      .def("transform", &Transformation3d::transform, py::arg("r"), "Returns transformed point")
      .def("inverted", &Transformation3d::inverted, "Returns inverted transformation")
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * Rotation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(Rotation3d() * py::self)
      .def(UniformScale3d() * py::self)
      .def("matrix3d",
          [](Transformation3d& transformation3d){
              py::array_t<double, py::array::f_style> result({3,3});
              auto matrix =transformation3d.get_underlying_matrix();
              std::copy(matrix.data(),matrix.data()+9, result.mutable_data());
              return result;
          },
           "Returns non-translational part of transformation"
          )
      .def("vector3d",
          [](Transformation3d& transformation3d){
            return transformation3d.get_translation();
          },
          "Returns translational part of transformation"
          );

}
