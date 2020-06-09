#include "xmol/v1/geom/alignment-impl.h"
#include "xmol/v1/geom/alignment.h"
#include "xmol/v1/proxy/selections.h"
#include "xmol/v1/proxy/spans.h"

using namespace xmol::v1::geom;

affine::Transformation3d xmol::v1::geom::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
affine::Transformation3d xmol::v1::geom::calc_alignment(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
affine::Transformation3d xmol::v1::geom::calc_alignment(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
affine::Transformation3d xmol::v1::geom::calc_alignment(proxy::CoordSelection& reference,
                                                        proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}

double xmol::v1::geom::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::geom::calc_rmsd(proxy::CoordSpan& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::geom::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::v1::geom::calc_rmsd(proxy::CoordSelection& reference, proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
