#include "init.h"
#include "xmol/geometry/alignment.h"


using namespace xmol::geometry;

void pyxmolpp::geometry::init_alignment(pybind11::module& geometry) {
  geometry.def("calc_alignment",xmol::geometry::calc_alignment);
  geometry.def("calc_geom_center",xmol::geometry::calc_geom_center);
}