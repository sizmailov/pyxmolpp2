#include "xmol/proxy/proxy.h"
#include "xmol/Frame.h"
#include "xmol/proxy/SelectionImpl.h"

using namespace xmol::proxy;

ResidueRef MoleculeRef::add_residue() { return ResidueRef(frame().add_residue(*m_molecule)); }

AtomRef ResidueRef::add_atom() { return proxy::AtomRef(frame().add_atom(*m_residue)); }

xmol::proxy::AtomRef::AtomRef(BaseAtom* ptr, BaseAtom* end) : m_atom(ptr) {
  if (ptr != end) {
    m_coord = &ptr->residue->molecule->frame->crd(*ptr);
  }
}
AtomRef::AtomRef(BaseAtom& atom) : m_coord(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}

smart::MoleculeSmartRef MoleculeRef::smart() { return smart::MoleculeSmartRef(*this); }
std::optional<ResidueRef> MoleculeRef::operator[](const xmol::ResidueId& id) {
  // todo: benchmark vs mappings (number of residues might be high)
  for (auto& r : residues()) {
    if (r.id() == id) {
      return r;
    }
  }
  return {};
}
std::optional<ResidueRef> MoleculeRef::operator[](residueSerial_t id) { return (*this)[ResidueId(id)]; }
xmol::MoleculeIndex MoleculeRef::index() const noexcept { return frame().index_of(*m_molecule); }
xmol::ResidueIndex ResidueRef::index() const noexcept { return frame().index_of(*m_residue); }
xmol::AtomIndex AtomRef::index() const noexcept { return frame().index_of(*m_atom); }
smart::ResidueSmartRef ResidueRef::smart() { return smart::ResidueSmartRef(*this); }
std::optional<AtomRef> ResidueRef::operator[](const xmol::AtomName& name) {
  for (auto& a : atoms()) {
    if (a.name() == name) {
      return a;
    }
  }
  return {};
}
std::optional<AtomRef> ResidueRef::operator[](const char* name) { return operator[](AtomName(name)); }
std::optional<AtomRef> ResidueRef::operator[](const std::string& name) { return operator[](AtomName(name)); }

smart::AtomSmartRef AtomRef::smart() { return smart::AtomSmartRef(*this); }

template class xmol::proxy::Selection<CoordRef>;
template class xmol::proxy::Selection<AtomRef>;
template class xmol::proxy::Selection<ResidueRef>;
template class xmol::proxy::Selection<MoleculeRef>;

CoordRef::CoordRef(XYZ& coord) : m_coord(&coord) {}
CoordRef::CoordRef(xmol::XYZ* ptr, xmol::XYZ*) : m_coord(ptr) {}
