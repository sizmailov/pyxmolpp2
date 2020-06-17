#include "xmol/future/span.h"
#include "xmol/fwd.h"
#include "xmol/geom/fwd.h"
#include "xmol/geom/affine/Transformation3d.h"

namespace xmol::algo {

[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::AtomSpan& reference, proxy::AtomSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::AtomSpan& reference, proxy::AtomSelection& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::AtomSelection& reference, proxy::AtomSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(proxy::AtomSelection& reference, proxy::AtomSelection& variable);

[[nodiscard]] double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable);
[[nodiscard]] double calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable);
[[nodiscard]] double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable);
[[nodiscard]] double calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable);

[[nodiscard]] double calc_weighted_rmsd(proxy::AtomSpan& reference, proxy::AtomSpan& variable);
[[nodiscard]] double calc_weighted_rmsd(proxy::AtomSpan& reference, proxy::AtomSelection& variable);
[[nodiscard]] double calc_weighted_rmsd(proxy::AtomSelection& reference, proxy::AtomSpan& variable);
[[nodiscard]] double calc_weighted_rmsd(proxy::AtomSelection& reference, proxy::AtomSelection& variable);

[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSelection& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSpan& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSelection& reference, const future::Span<double>& mass);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::CoordSpan& reference, const future::Span<double>& mass);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::AtomSelection& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(proxy::AtomSpan& reference);

} // namespace xmol::geom