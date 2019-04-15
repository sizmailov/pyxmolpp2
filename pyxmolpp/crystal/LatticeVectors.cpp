#include "init.h"

#include "xmol/crystal/LatticeVectors.h"

using namespace xmol::crystal;
using namespace xmol::geometry;

void pyxmolpp::crystal::init_lattice_vectors(py::module& crystal) {
  py::class_<LatticeVectors>(crystal,"LatticeVectors")
      .def(py::init<const XYZ&,const XYZ&,const XYZ&>(),py::arg("v1"),py::arg("v2"),py::arg("v3"),
          "Construct lattice vectors explicitly")
      .def(py::init<double,double,double,AngleValue,AngleValue,AngleValue>(),
          py::arg("a"),py::arg("b"),py::arg("c"),
          py::arg("alpha"),
          py::arg("beta"),
          py::arg("gamma"),
          "Construct lattice vectors from cell parameters")
      .def("translate",
          &LatticeVectors::translate,
          py::arg("r"),
          py::arg("i"),
          py::arg("j"),
          py::arg("k"),
           R"(Returns :math:`\vec r + i  \vec v_1 + j \vec  v_2 + k\vec  v_3`)"
      )
      .def("get_shift",
          &LatticeVectors::get_shift,
          py::arg("i"),
          py::arg("j"),
          py::arg("k"),
           R"(Returns :math:`i  \vec v_1 + j \vec  v_2 + k\vec  v_3`)"
          )
      .def("scale_by",
          &LatticeVectors::scale_by,py::arg("factor"),
          "Scale lattice vectors by given factor")
      .def("__getitem__",(const XYZ& (LatticeVectors::*)(int i) const)&LatticeVectors::operator[],py::arg("i"))
      ;
}
