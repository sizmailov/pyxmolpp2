#pragma once
#include <stdexcept>

/// Atomic-agnostic geom procedures
namespace xmol::geom {

class GeomError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class XYZ;
class UnitCell;
struct AngleValue;

namespace affine {
class Translation3d;
class Transformation3d;
class UniformScale3d;
class Rotation3d;
} // namespace affine

} // namespace xmol::geom