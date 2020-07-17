#include "xmol/proxy/references/AtomRef.h"
#include "xmol/Frame.h"

using namespace xmol::proxy;

xmol::AtomIndex AtomRef::index() const { return frame().index_of(*atom_ptr()); }