#include "xmol/proxy/references/ConstAtomRef.h"
#include "xmol/Frame.h"

using namespace xmol::proxy;

xmol::AtomIndex ConstAtomRef::index() const {
  check_invariants("index");
  return frame().index_of(*m_atom);
}