#include "xmol/proxy/smart/MoleculeSmartSpan.h"
#include "xmol/proxy/smart/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

void xmol::proxy::smart::MoleculeSmartSpan::on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end,
                                                                       BaseMolecule* to_begin) {
  if (m_is_split || m_span.m_begin >= from_end || from_begin >= m_span.m_end) {
    return;
  }
  if (from_begin <= m_span.m_begin && m_span.m_end <= from_end) {
    m_span.rebase(from_begin, to_begin);
  } else {
    m_is_split = true;
  }
}

xmol::proxy::smart::MoleculeSmartSpan::MoleculeSmartSpan(xmol::proxy::MoleculeSpan sel)
    : FrameObserver(sel.frame_ptr()), m_span(sel) {
  if (m_span.frame_ptr()) {
    m_span.frame_ptr()->reg(*this);
  }
}


template class xmol::proxy::smart::FrameObserver<MoleculeSmartSpan>;