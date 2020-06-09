#include "../fwd.h"
#include "affine/Transformation3d.h"

namespace xmol::v1::geom {

affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

} // namespace xmol::v1::geom