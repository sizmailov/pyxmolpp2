#include "xmol/proxy/smart/MoleculeSmartSelection.h"
#include "xmol/Frame.h"
#include "xmol/proxy/smart/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

struct MoleculeSmartSelection::MoleculeRefLessThanComparator {
  bool operator()(MoleculeRef& a, BaseMolecule* ptr) { return a.m_molecule < ptr; }
  bool operator()(BaseMolecule* ptr, MoleculeRef& a) { return ptr < a.m_molecule; }
};

void MoleculeSmartSelection::on_base_molecules_move(BaseMolecule *from_begin, BaseMolecule *from_end, BaseMolecule *to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, MoleculeRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, MoleculeRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->m_molecule);
    assert(it->m_molecule < from_end);
    it->m_molecule = to_begin + (it->m_molecule - from_begin);
  }
}

xmol::proxy::smart::MoleculeSmartSelection::MoleculeSmartSelection(xmol::proxy::MoleculeSelection sel)
    : FrameObserver(sel.frame_ptr()), m_selection(std::move(sel)) {
  if (m_selection.frame_ptr()) {
    m_selection.frame_ptr()->reg(*this);
  }
}

template class xmol::proxy::smart::FrameObserver<MoleculeSmartSelection>;
