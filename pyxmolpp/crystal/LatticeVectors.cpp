#include "init.h"
#include "pybind11/stl.h"
#include "xmol/geom/UnitCell.h"

using namespace xmol::geom;

void pyxmolpp::crystal::init_lattice_vectors(py::module& crystal) {
  auto&& pyUnitCell = py::class_<UnitCell>(crystal,"UnitCell");
  py::class_<UnitCell::ClosestImage>(pyUnitCell,"UnitCell")
      .def(py::init<>())
      .def_readwrite("shift_int", &UnitCell::ClosestImage::shift_int)
      .def_readwrite("shift", &UnitCell::ClosestImage::shift)
      .def_readwrite("distance", &UnitCell::ClosestImage::distance)
      .def_readwrite("pos", &UnitCell::ClosestImage::pos
   ;

  pyUnitCell
      .def(py::init<const XYZ&,const XYZ&,const XYZ&>(),py::arg("v1"),py::arg("v2"),py::arg("v3"),
          "Construct lattice vectors explicitly")
      .def(py::init<double,double,double,AngleValue,AngleValue,AngleValue>(),
          py::arg("a"),py::arg("b"),py::arg("c"),
          py::arg("alpha"),
          py::arg("beta"),
          py::arg("gamma"),
          "Construct lattice vectors from cell parameters")
      .def("translation_vector",
          &UnitCell::translation_vector,
          py::arg("i"),
          py::arg("j"),
          py::arg("k"),
           R"(Returns :math:`i  \vec v_1 + j \vec  v_2 + k\vec  v_3`)"
          )
      .def("closest_image_to",
           &UnitCell::closest_image_to,
           py::arg("i"),
           py::arg("j"),
           py::arg("k"),
           R"(Find closest periodic image

    :param ref: reference point
    :param var: variable point
    :returns: `shift` that minimizes :math:`(ref-(var+shift))^2`)")
      .def("scale_by",
          &UnitCell::scale_by,py::arg("factor"),
          "Scale lattice vectors by given factor")
      .def("__getitem__",(const XYZ& (UnitCell::*)(int i) const)&UnitCell::operator[],py::arg("i"))
      ;
}
