#include "xmol/v1/proxy/smart/selections.h"
#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/smart/FrameObserverImpl.h"

using namespace xmol::v1::proxy::smart;

struct AtomSmartSelection::AtomRefLessThanComparator {
  bool operator()(AtomRef& a, XYZ* ptr) { return a.m_coords < ptr; }
  bool operator()(XYZ* ptr, AtomRef& a) { return ptr < a.m_coords; }
  bool operator()(AtomRef& a, BaseAtom* ptr) { return a.m_atom < ptr; }
  bool operator()(BaseAtom* ptr, AtomRef& a) { return ptr < a.m_atom; }
};

void AtomSmartSelection::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  auto it = std::lower_bound(m_data.begin(), m_data.end(), from_begin, AtomRefLessThanComparator{});
  auto it_end = std::upper_bound(m_data.begin(), m_data.end(), from_end, AtomRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_coords);
    assert(it->m_coords < from_end);
    it->m_coords = to_begin + (it->m_coords - from_begin);
  }
}

void AtomSmartSelection::on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  auto it = std::lower_bound(m_data.begin(), m_data.end(), from_begin, AtomRefLessThanComparator{});
  auto it_end = std::upper_bound(m_data.begin(), m_data.end(), from_end, AtomRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_atom);
    assert(it->m_atom < from_end);
    it->m_atom = to_begin + (it->m_atom - from_begin);
  }
}

xmol::v1::proxy::smart::AtomSmartSelection::AtomSmartSelection(xmol::v1::proxy::AtomSelection sel)
    : AtomSelection(std::move(sel)), FrameObserver(frame_ptr()) {
  if (m_frame) {
    m_frame->reg(*this);
  }
}

template class xmol::v1::proxy::smart::FrameObserver<AtomSmartSelection>;
