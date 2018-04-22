#include "init.h"

void pyxmolpp::init_geometry(pybind11::module& m) {
  geometry::init_XYZ(m);
  geometry::init_AngleValue(m);
  geometry::init_basic(m);
  geometry::init_Transformation3d(m);
  geometry::init_alignment(m);
}
