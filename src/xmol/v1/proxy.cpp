#include "xmol/v1/proxy/proxy.h"
#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/SelectionImpl.h"

using namespace xmol::v1::proxy;

ResidueRef MoleculeRef::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return ResidueRef(frame().add_residue(*m_molecule, residueName, residueId));
}

AtomRef ResidueRef::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return proxy::AtomRef(frame().add_atom(*m_residue, atomName, atomId));
}

xmol::v1::proxy::AtomRef::AtomRef(BaseAtom* ptr, BaseAtom* end) : m_atom(ptr) {
  if (ptr != end) {
    m_coords = &ptr->residue->molecule->frame->crd(*ptr);
  }
}
AtomRef::AtomRef(BaseAtom& atom) : m_coords(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}


template class xmol::v1::proxy::Selection<AtomRef>;
template class xmol::v1::proxy::Selection<ResidueRef>;
template class xmol::v1::proxy::Selection<MoleculeRef>;