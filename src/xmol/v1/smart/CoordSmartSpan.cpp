#include "xmol/proxy/smart/CoordSmartSpan.h"
#include "xmol/proxy/smart/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

xmol::proxy::smart::CoordSmartSpan::CoordSmartSpan(xmol::proxy::CoordSpan span)
    : FrameObserver(span.m_frame), m_span(span) {
  if (m_span.m_frame) {
    m_span.m_frame->reg(*this);
  }
}

void xmol::proxy::smart::CoordSmartSpan::on_coordinates_move(XYZ *from_begin, XYZ *from_end, XYZ *to_begin) {
  if (m_is_split || m_span.m_begin >= from_end || from_begin >= m_span.m_end) {
    return;
  }
  if (from_begin <= m_span.m_begin && m_span.m_end <= from_end) {
    m_span.rebase(from_begin, to_begin);
  } else {
    m_is_split = true;
  }
}

template class xmol::proxy::smart::FrameObserver<CoordSmartSpan>;