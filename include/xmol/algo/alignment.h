#include "xmol/future/span.h"
#include "xmol/fwd.h"
#include "xmol/geom/fwd.h"
#include "xmol/geom/affine/Transformation3d.h"

namespace xmol::algo {

[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::CoordSpan& reference,
                                                            const proxy::CoordSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::CoordSpan& reference,
                                                            const proxy::CoordSelection& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::CoordSelection& reference,
                                                            const proxy::CoordSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::CoordSelection& reference,
                                                            const proxy::CoordSelection& variable);

[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::AtomSpan& reference,
                                                            const proxy::AtomSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::AtomSpan& reference,
                                                            const proxy::AtomSelection& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::AtomSelection& reference,
                                                            const proxy::AtomSpan& variable);
[[nodiscard]] geom::affine::Transformation3d calc_alignment(const proxy::AtomSelection& reference,
                                                            const proxy::AtomSelection& variable);

[[nodiscard]] double calc_rmsd(const proxy::CoordSpan& reference, const proxy::CoordSpan& variable);
[[nodiscard]] double calc_rmsd(const proxy::CoordSpan& reference, const proxy::CoordSelection& variable);
[[nodiscard]] double calc_rmsd(const proxy::CoordSelection& reference, const proxy::CoordSpan& variable);
[[nodiscard]] double calc_rmsd(const proxy::CoordSelection& reference, const proxy::CoordSelection& variable);

[[nodiscard]] double calc_weighted_rmsd(const proxy::AtomSpan& reference, const proxy::AtomSpan& variable);
[[nodiscard]] double calc_weighted_rmsd(const proxy::AtomSpan& reference, const proxy::AtomSelection& variable);
[[nodiscard]] double calc_weighted_rmsd(const proxy::AtomSelection& reference, const proxy::AtomSpan& variable);
[[nodiscard]] double calc_weighted_rmsd(const proxy::AtomSelection& reference, const proxy::AtomSelection& variable);

[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::CoordSelection& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::CoordSpan& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::CoordSelection& reference,
                                                  const future::Span<double>& mass);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::CoordSpan& reference, const future::Span<double>& mass);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::AtomSelection& reference);
[[nodiscard]] Eigen::Matrix3d calc_inertia_tensor(const proxy::AtomSpan& reference);

} // namespace xmol::geom