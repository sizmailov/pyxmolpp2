#include "init.h"

#include <pybind11/operators.h>

#include "xmol/geometry/Transformation3d.h"


using namespace xmol::geometry;

void pyxmolpp::geometry::init_Transformation3d(pybind11::module& geometry) {

  auto&& pyTranslation3d = py::class_<Translation3d>(geometry,"Translation3d");
  auto&& pyUniformScale3d = py::class_<UniformScale3d>(geometry,"UniformScale3d");
  auto&& pyRotation3d = py::class_<Rotation3d>(geometry,"Rotation3d");
  auto&& pyTransformation3d = py::class_<Transformation3d>(geometry,"Transformation3d");

  pyTranslation3d
      .def(py::init<>())
      .def(py::init<XYZ>())
      .def("transform",&Translation3d::transform)
      .def("inverted",&Translation3d::inverted)
      .def("dr",&Translation3d::dr)
      .def(py::self * py::self)
      ;

  pyUniformScale3d
      .def(py::init<>())
      .def(py::init<double>())
      .def("transform",&UniformScale3d::transform)
      .def("inverted",&UniformScale3d::inverted)
      .def_property_readonly("scale",&UniformScale3d::scale)
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(Translation3d() * py::self)
  ;

  pyRotation3d
      .def(py::init<>())
      .def(py::init<XYZ,AngleValue>())
      .def("transform",&Rotation3d::transform)
      .def("inverted",&Rotation3d::inverted)
      .def("axis",&Rotation3d::axis)
      .def("theta",&Rotation3d::theta)
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(UniformScale3d() * py::self)
      ;

  pyTransformation3d
      .def(py::init<>())
      .def(py::init<Rotation3d,Translation3d>())
      .def("transform",&Transformation3d::transform)
      .def("inverted",&Transformation3d::inverted)
      .def(py::self * py::self)
      .def(py::self * Translation3d())
      .def(py::self * Rotation3d())
      .def(py::self * UniformScale3d())
      .def(Translation3d() * py::self)
      .def(Rotation3d() * py::self)
      .def(UniformScale3d() * py::self)
      ;

}
