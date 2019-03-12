#include "init.h"

#include "xmol/geometry/XYZ.h"
#include "xmol/geometry/Transformation3d.h"

#include <pybind11/operators.h>
#include <pybind11/numpy.h>


using namespace xmol::geometry;

void pyxmolpp::geometry::init_XYZ(pybind11::module& geometry) {

  py::class_<XYZ>(geometry,"XYZ","Cartesian 3d vector")
      .def(py::init<>(),"Equivalent to XYZ(0,0,0)")
      .def(py::init<double,double,double>(),py::arg("x"),py::arg("y"),py::arg("z"),"Initialize with x, y, z")
      .def(py::self+py::self)
      .def(py::self-py::self)
      .def(-py::self)
      .def(py::self/double())
      .def(py::self*double())
      .def(double()*py::self)
      .def("dot",&XYZ::dot, py::arg("rhs"), "Returns dot product")
      .def("cross",&XYZ::cross, py::arg("rhs"), "Returns cross product")
      .def("__repr__",[](XYZ& xyz){
        return "["+std::to_string(xyz.x())+", "+std::to_string(xyz.y())+", "+std::to_string(xyz.z())+"]";
      })
      .def("__str__",[](XYZ& xyz){
        return "["+std::to_string(xyz.x())+", "+std::to_string(xyz.y())+", "+std::to_string(xyz.z())+"]";
      })
      .def_property("x",&XYZ::x,&XYZ::set_x,"x coordinate")
      .def_property("y",&XYZ::y,&XYZ::set_y,"y coordinate")
      .def_property("z",&XYZ::z,&XYZ::set_z,"z coordinate")
      .def("len",&XYZ::len, "Returns vector length")
      .def("len2",&XYZ::len2, "Returns vector length squared")
      .def_property_readonly("to_np",[](XYZ& xyz){
          py::array_t<double> result(3);
          result.mutable_data()[0] = xyz.x();
          result.mutable_data()[1] = xyz.y();
          result.mutable_data()[2] = xyz.z();
          return result;
        },
            "Returns XYZ object as numpy.array")
      ;
}
