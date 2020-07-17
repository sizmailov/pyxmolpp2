#include "xmol/proxy/smart/CoordSmartSelection.h"
#include "xmol/Frame.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

struct CoordSmartSelection::CoordRefLessThanComparator {
  bool operator()(CoordRef& a, XYZ* ptr) { return a.coord_ptr() < ptr; }
  bool operator()(XYZ* ptr, CoordRef& a) { return ptr < a.coord_ptr(); }
};

void CoordSmartSelection::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, CoordRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, CoordRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->coord_ptr());
    assert(it->coord_ptr() < from_end);
    it->rebase(from_begin, to_begin);
  }
}

xmol::proxy::smart::CoordSmartSelection::CoordSmartSelection(xmol::proxy::CoordSelection sel)
    : FrameObserver(sel.m_frame), m_selection(std::move(sel)) {
  if (m_selection.m_frame) {
    m_selection.m_frame->reg(*this);
  }
}
xmol::geom::affine::Transformation3d CoordSmartSelection::alignment_to(xmol::proxy::CoordSelection& other) {
  check_precondition("alignment_to()");
  return m_selection.alignment_to(other);
}

xmol::geom::affine::Transformation3d CoordSmartSelection::alignment_to(xmol::proxy::CoordSpan& other) {
  check_precondition("alignment_to()");
  return m_selection.alignment_to(other);
}

template class xmol::proxy::FrameObserver<CoordSmartSelection>;
