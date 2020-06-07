#include "xmol/v1/proxy/smart/MoleculeSmartSpan.h"

using namespace xmol::v1::proxy::smart;

void xmol::v1::proxy::smart::MoleculeSmartSpan::on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end,
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
