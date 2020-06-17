#include "xmol/proxy/smart/AtomSmartSelection.h"
#include "xmol/Frame.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

struct AtomSmartSelection::AtomRefLessThanComparator {
  bool operator()(AtomRef& a, XYZ* ptr) { return a.m_coord < ptr; }
  bool operator()(XYZ* ptr, AtomRef& a) { return ptr < a.m_coord; }
  bool operator()(AtomRef& a, BaseAtom* ptr) { return a.m_atom < ptr; }
  bool operator()(BaseAtom* ptr, AtomRef& a) { return ptr < a.m_atom; }
};

void AtomSmartSelection::on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, AtomRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, AtomRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_coord);
    assert(it->m_coord < from_end);
    it->m_coord = to_begin + (it->m_coord - from_begin);
  }
}

void AtomSmartSelection::on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, AtomRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, AtomRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_atom);
    assert(it->m_atom < from_end);
    it->m_atom = to_begin + (it->m_atom - from_begin);
  }
}

xmol::proxy::smart::AtomSmartSelection::AtomSmartSelection(xmol::proxy::AtomSelection sel)
    : FrameObserver(sel.frame_ptr()), m_selection(std::move(sel)) {
  if (m_selection.frame_ptr()) {
    m_selection.frame_ptr()->reg(*this);
  }
}

auto AtomSmartSelection::inertia_tensor() -> Eigen::Matrix3d {
  check_precondition("inertia_tensor()");
  return m_selection.inertia_tensor();
}

auto AtomSmartSelection::alignment_to(xmol::proxy::AtomSpan& rhs, bool weighted) -> geom::affine::Transformation3d {
  check_precondition("alignment_to()");
  return m_selection.alignment_to(rhs, weighted);
}

auto AtomSmartSelection::alignment_to(xmol::proxy::AtomSelection& rhs, bool weighted) -> geom::affine::Transformation3d {
  check_precondition("alignment_to()");
  return m_selection.alignment_to(rhs, weighted);
}

template class xmol::proxy::smart::FrameObserver<AtomSmartSelection>;
