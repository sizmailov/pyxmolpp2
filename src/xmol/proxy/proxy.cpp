#include "xmol/proxy/proxy.h"
#include "xmol/Frame.h"
#include "xmol/proxy/SelectionImpl.h"

using namespace xmol::proxy;

ResidueRef MoleculeRef::add_residue() const { return ResidueRef(frame().add_residue(*mol_ptr())); }

AtomRef ResidueRef::add_atom() const { return proxy::AtomRef(frame().add_atom(*res_ptr())); }

xmol::proxy::ConstAtomRef::ConstAtomRef(BaseAtom* ptr, BaseAtom* end) : m_atom(ptr) {
  if (ptr != end) {
    m_coord = &ptr->residue->molecule->frame->crd(*ptr);
  }
}

ConstAtomRef::ConstAtomRef(BaseAtom& atom) : m_coord(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}

MoleculeSmartRef MoleculeRef::smart() const { return MoleculeSmartRef(*this); }
std::optional<ResidueRef> MoleculeRef::operator[](const xmol::ResidueId& id) const {
  // todo: benchmark vs mappings (number of residues might be high)
  for (auto& r : residues()) {
    if (r.id() == id) {
      return r;
    }
  }
  return {};
}
std::optional<ResidueRef> MoleculeRef::operator[](ResidueIdSerial id) const { return (*this)[ResidueId(id)]; }
xmol::MoleculeIndex MoleculeRef::index() const { return frame().index_of(*mol_ptr()); }
xmol::ResidueIndex ResidueRef::index() const { return frame().index_of(*res_ptr()); }
ResidueSmartRef ResidueRef::smart() const { return ResidueSmartRef(*this); }
std::optional<AtomRef> ResidueRef::operator[](const xmol::AtomName& name) const {
  for (auto& a : atoms()) {
    if (a.name() == name) {
      return a;
    }
  }
  return {};
}
std::optional<AtomRef> ResidueRef::operator[](const char* name) const { return operator[](AtomName(name)); }
std::optional<AtomRef> ResidueRef::operator[](const std::string& name) const { return operator[](AtomName(name)); }

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
