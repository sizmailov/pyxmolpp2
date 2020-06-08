#pragma once
#include <stdexcept>

/// Atomic-agnostic geom procedures
namespace xmol::v1::geom {

class GeomError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class XYZ;
struct AngleValue;

namespace affine {}

} // namespace xmol::v1::geom