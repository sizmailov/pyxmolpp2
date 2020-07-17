#include "xmol/proxy/smart/MoleculeSmartSelection.h"
#include "xmol/Frame.h"
#include "xmol/proxy/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

struct MoleculeSmartSelection::MoleculeRefLessThanComparator {
  bool operator()(MoleculeRef& a, BaseMolecule* ptr) { return a.mol_ptr() < ptr; }
  bool operator()(BaseMolecule* ptr, MoleculeRef& a) { return ptr < a.mol_ptr(); }
};

void MoleculeSmartSelection::on_base_molecules_move(BaseMolecule *from_begin, BaseMolecule *from_end, BaseMolecule *to_begin) {
  auto it =
      std::lower_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_begin, MoleculeRefLessThanComparator{});
  auto it_end =
      std::upper_bound(m_selection.m_data.begin(), m_selection.m_data.end(), from_end, MoleculeRefLessThanComparator{});
  for (; it != it_end; ++it) {
    assert(from_begin <= it->mol_ptr());
    assert(it->mol_ptr() < from_end);
    it->rebase(from_begin, to_begin);
  }
}

xmol::proxy::smart::MoleculeSmartSelection::MoleculeSmartSelection(xmol::proxy::MoleculeSelection sel)
    : FrameObserver(sel.frame_ptr()), m_selection(std::move(sel)) {
  if (m_selection.frame_ptr()) {
    m_selection.frame_ptr()->reg(*this);
  }
}

template class xmol::proxy::FrameObserver<MoleculeSmartSelection>;
