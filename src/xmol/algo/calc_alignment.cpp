#include "xmol/algo/alignment-impl.h"
#include "xmol/algo/alignment.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/spans.h"

using namespace xmol::algo;
using namespace xmol::geom::affine;

Transformation3d xmol::algo::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(proxy::CoordSelection& reference, proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}

double xmol::algo::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(xmol::proxy::CoordSelection& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(xmol::proxy::CoordSpan& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}
Eigen::Matrix3d xmol::algo::calc_inertia_tensor(xmol::proxy::CoordSelection& coords, const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass_map(mass.m_begin, 1, mass.size());
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(xmol::proxy::CoordSpan& coords, const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass_map(mass.m_begin, 1, mass.size());
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}