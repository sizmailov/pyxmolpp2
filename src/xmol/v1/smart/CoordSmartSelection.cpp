#include "xmol/v1/proxy/smart/CoordSmartSelection.h"
#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/smart/FrameObserverImpl.h"

using namespace xmol::v1::proxy::smart;

struct CoordSmartSelection::CoordRefLessThanComparator {
  bool operator()(CoordRef& a, XYZ* ptr) { return a.m_coord < ptr; }
  bool operator()(XYZ* ptr, CoordRef& a) { return ptr < a.m_coord; }
};


void CoordSmartSelection::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, CoordRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, CoordRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_coord);
    assert(it->m_coord < from_end);
    it->m_coord = to_begin + (it->m_coord - from_begin);
  }
}

xmol::v1::proxy::smart::CoordSmartSelection::CoordSmartSelection(xmol::v1::proxy::CoordSelection sel)
    : FrameObserver(sel.m_frame), m_selection(std::move(sel)) {
  if (m_selection.m_frame) {
    m_selection.m_frame->reg(*this);
  }
}

template class xmol::v1::proxy::smart::FrameObserver<CoordSmartSelection>;
