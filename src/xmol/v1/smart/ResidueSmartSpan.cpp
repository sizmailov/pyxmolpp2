#include "xmol/v1/proxy/smart/ResidueSmartSpan.h"
#include "xmol/v1/proxy/smart/FrameObserverImpl.h"

using namespace xmol::v1::proxy::smart;

void xmol::v1::proxy::smart::ResidueSmartSpan::on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end,
                                                                     BaseResidue* to_begin) {
  if (m_is_split || m_span.m_begin >= from_end || from_begin >= m_span.m_end) {
    return;
  }
  if (from_begin <= m_span.m_begin && m_span.m_end <= from_end) {
    m_span.rebase(from_begin, to_begin);
  } else {
    m_is_split = true;
  }
}

xmol::v1::proxy::smart::ResidueSmartSpan::ResidueSmartSpan(xmol::v1::proxy::ResidueRefSpan sel)
    : FrameObserver(sel.frame_ptr()), m_span(sel) {
  if (m_span.frame_ptr()) {
    m_span.frame_ptr()->reg(*this);
  }
}

template class xmol::v1::proxy::smart::FrameObserver<ResidueSmartSpan>;