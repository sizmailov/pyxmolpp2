#include "xmol/proxy/smart/AtomSmartSpan.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/FrameObserverImpl.h"

using namespace xmol::proxy::smart;

void xmol::proxy::smart::AtomSmartSpan::on_base_atoms_move(xmol::BaseAtom* from_begin,
                                                               xmol::BaseAtom* from_end,
                                                               xmol::BaseAtom* to_begin) {
  if (m_is_split || m_span.m_begin >= from_end || from_begin >= m_span.m_end) {
    return;
  }
  if (from_begin <= m_span.m_begin && m_span.m_end <= from_end) {
    m_span.rebase(from_begin, to_begin);
  } else {
    m_is_split = true;
  }
}

xmol::proxy::smart::AtomSmartSpan::AtomSmartSpan(xmol::proxy::AtomSpan sel)
    : FrameObserver(sel.frame_ptr()), m_span(sel) {
  if (m_span.frame_ptr()) {
    m_span.frame_ptr()->reg(*this);
  }
}

auto AtomSmartSpan::inertia_tensor() -> Eigen::Matrix3d {
  check_precondition("inertia_tensor()");
  return m_span.inertia_tensor();
}

auto AtomSmartSpan::alignment_to(xmol::proxy::AtomSpan& rhs, bool weighted) -> geom::affine::Transformation3d {
  check_precondition("alignment_to()");
  return m_span.alignment_to(rhs, weighted);
}

auto AtomSmartSpan::alignment_to(xmol::proxy::AtomSelection& rhs, bool weighted) -> geom::affine::Transformation3d {
  check_precondition("alignment_to()");
  return m_span.alignment_to(rhs, weighted);
}

template class xmol::proxy::smart::FrameObserver<AtomSmartSpan>;