#include "xmol/v1/algo/alignment-impl.h"
#include "xmol/v1/algo/alignment.h"
#include "xmol/v1/proxy/selections.h"
#include "xmol/v1/proxy/spans.h"

using namespace xmol::v1::algo;
using namespace xmol::v1::geom::affine;

Transformation3d xmol::v1::algo::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::v1::algo::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::v1::algo::calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::v1::algo::calc_alignment(proxy::CoordSelection& reference,
                                                        proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}

double xmol::v1::algo::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::algo::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::algo::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::algo::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}

Eigen::Matrix3d xmol::v1::algo::calc_inertia_tensor(xmol::v1::proxy::CoordSelection& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}

Eigen::Matrix3d xmol::v1::algo::calc_inertia_tensor(xmol::v1::proxy::CoordSpan& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}
Eigen::Matrix3d xmol::v1::algo::calc_inertia_tensor(xmol::v1::proxy::CoordSelection& coords,
                                                    const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, 1, Eigen::Dynamic>> mass_map(mass.m_begin, 1, mass.size());
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}

Eigen::Matrix3d xmol::v1::algo::calc_inertia_tensor(xmol::v1::proxy::CoordSpan& coords,
                                                    const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, 1, Eigen::Dynamic>> mass_map(mass.m_begin, 1, mass.size());
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}