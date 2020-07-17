#include "xmol/proxy/references/AtomRef.h"
#include "xmol/proxy/references/AtomSmartRef.h"
#include "xmol/Frame.h"

using namespace xmol::proxy;

template <typename Derived> xmol::AtomIndex api::ConstAtomAPI<Derived>::index() const {
  return frame().index_of(*derived().atom_ptr());
}

template class api::ConstAtomAPI<AtomRef>;
template class api::ConstAtomAPI<AtomSmartRef>;