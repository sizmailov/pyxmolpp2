#include "xmol/v1/proxy/smart/AtomSmartSpan.h"

using namespace xmol::v1::proxy::smart;

void xmol::v1::proxy::smart::AtomSmartSpan::on_base_atoms_move(xmol::v1::BaseAtom* from_begin,
                                                               xmol::v1::BaseAtom* from_end,
                                                               xmol::v1::BaseAtom* to_begin) {
  if (m_is_split || m_span.m_begin >= from_end || from_begin >= m_span.m_end) {
    return;
  }
  if (from_begin <= m_span.m_begin && m_span.m_end <= from_end) {
    m_span.rebase(from_begin, to_begin);
  } else {
    m_is_split = true;
  }
}
