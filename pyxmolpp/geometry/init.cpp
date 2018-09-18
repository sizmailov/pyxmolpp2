#include "init.h"
#include "pybind11/stl_bind.h"

void pyxmolpp::init_geometry(pybind11::module& m) {
  geometry::init_exceptions(m);
  geometry::init_XYZ(m);
  py::bind_vector<std::vector<xmol::geometry::XYZ>>(m, "VectorXYZ");
  geometry::init_AngleValue(m);
  geometry::init_autocorrelation(m);
  geometry::init_basic(m);
  geometry::init_Transformation3d(m);
  geometry::init_alignment(m);
}
