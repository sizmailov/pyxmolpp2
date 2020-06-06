#include "xmol/v1/proxy/smart/ResidueSmartSelection.h"
#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/smart/FrameObserverImpl.h"

using namespace xmol::v1::proxy::smart;

struct ResidueSmartSelection::ResidueRefLessThanComparator {
  bool operator()(ResidueRef& a, BaseResidue* ptr) { return a.m_residue < ptr; }
  bool operator()(BaseResidue* ptr, ResidueRef& a) { return ptr < a.m_residue; }
};

void ResidueSmartSelection::on_base_residues_move(BaseResidue *from_begin, BaseResidue *from_end, BaseResidue *to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, ResidueRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, ResidueRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_residue);
    assert(it->m_residue < from_end);
    it->m_residue = to_begin + (it->m_residue - from_begin);
  }
}

xmol::v1::proxy::smart::ResidueSmartSelection::ResidueSmartSelection(xmol::v1::proxy::ResidueSelection sel)
    : FrameObserver(sel.frame_ptr()), m_selection(std::move(sel)) {
  if (m_selection.frame_ptr()) {
    m_selection.frame_ptr()->reg(*this);
  }
}

template class xmol::v1::proxy::smart::FrameObserver<ResidueSmartSelection>;
