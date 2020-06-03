#include "xmol/v1/proxy.h"
#include "xmol/v1/Frame.h"

using namespace xmol::v1;

proxy::Residue proxy::Molecule::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return proxy::Residue(frame().add_residue(*m_molecule, residueName, residueId));
}

proxy::Atom proxy::Residue::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return proxy::Atom(frame().add_atom(*m_residue, atomName, atomId));
}

xmol::v1::proxy::Atom::Atom(BaseAtom* ptr, BaseAtom* end) : m_atom(ptr) {
  if (ptr != end) {
    m_coords = &ptr->residue->molecule->frame->crd(*ptr);
  }
}
proxy::Atom::Atom(BaseAtom& atom) : m_coords(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}
