#pragma once
#include "../pyxmolpp.h"
#include "xmol/geometry/XYZ.h"

PYBIND11_MAKE_OPAQUE(std::vector<xmol::geometry::XYZ>);

namespace pyxmolpp {
namespace geometry {

void init_exceptions(py::module& geometry);
void init_alignment(py::module& geometry);
void init_AngleValue(py::module& geometry);
void init_autocorrelation(py::module& geometry);
void init_calc_sasa(py::module& geometry);
void init_basic(py::module& geometry);
void init_Transformation3d(py::module& geometry);
void init_XYZ(py::module& geometry);
void init_VectorXYZ(py::module& geometry);
}
}
