#include "xmol/v1/future/span.h"
#include "xmol/v1/fwd.h"
#include "xmol/v1/geom/fwd.h"
#include "xmol/v1/geom/affine/Transformation3d.h"

namespace xmol::v1::algo {

geom::affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
geom::affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
geom::affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
geom::affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSelection& reference);
Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSpan& reference);
Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSelection& reference, const future::Span<double>& mass);
Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSpan& reference, const future::Span<double>& mass);

} // namespace xmol::v1::geom