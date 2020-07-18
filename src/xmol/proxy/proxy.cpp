#include "xmol/proxy/proxy.h"
#include "xmol/Frame.h"
#include "xmol/proxy/SelectionImpl.h"

using namespace xmol::proxy;


xmol::proxy::ConstAtomRef::ConstAtomRef(BaseAtom* ptr, BaseAtom* end) : m_atom(ptr) {
  if (ptr != end) {
    m_coord = &ptr->residue->molecule->frame->crd(*ptr);
  }
}

ConstAtomRef::ConstAtomRef(BaseAtom& atom) : m_coord(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}

MoleculeSmartRef MoleculeRef::smart() const { return MoleculeSmartRef(*this); }
ResidueSmartRef ResidueRef::smart() const { return ResidueSmartRef(*this); }
AtomSmartRef AtomRef::smart() const { return AtomSmartRef(*this); }

template class xmol::proxy::Selection<CoordRef>;
template class xmol::proxy::Selection<AtomRef>;
template class xmol::proxy::Selection<ResidueRef>;
template class xmol::proxy::Selection<MoleculeRef>;

CoordRef::CoordRef(XYZ& coord) : m_cref(coord) {}
CoordRef::CoordRef(xmol::XYZ* ptr, xmol::XYZ*) : m_cref(ptr, nullptr) {}

ConstCoordRef::ConstCoordRef(XYZ& coord) : m_coord(&coord) {}
ConstCoordRef::ConstCoordRef(xmol::XYZ* ptr, xmol::XYZ*) : m_coord(ptr) {}

std::string xmol::proxy::to_string(const AtomRef& atom) {
  return to_string(const_cast<AtomRef&>(atom).residue()) + "." + atom.name().str();
}
std::string xmol::proxy::to_string(const ResidueRef& res) {
  return to_string(const_cast<ResidueRef&>(res).molecule()) + "." + res.name().str() + "-" + to_string(res.id());
}
