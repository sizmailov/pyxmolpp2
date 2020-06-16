#pragma once
#include "xmol/geom/affine/Transformation3d.h"
#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void populate(pybind11::class_<xmol::geom::affine::Rotation3d>& pyRotation);
void populate(pybind11::class_<xmol::geom::affine::Transformation3d>& pyTransformation);
void populate(pybind11::class_<xmol::geom::affine::Translation3d>& pyTranslation);
void populate(pybind11::class_<xmol::geom::affine::UniformScale3d>& pyUniformScale);

}