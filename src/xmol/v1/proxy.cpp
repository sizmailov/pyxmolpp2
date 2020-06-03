#include "xmol/v1/proxy.h"
#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy-span-impl.h"

using namespace xmol::v1;

const MoleculeName& proxy::Molecule::name() const {
  assert(m_molecule);
  assert(m_molecule->frame);
  return m_molecule->name;
}
void proxy::Molecule::name(const MoleculeName& name) {
  assert(m_molecule);
  assert(m_molecule->frame);
  m_molecule->name = name;
}
bool proxy::Molecule::empty() const {
  assert(m_molecule);
  assert(m_molecule->frame);
  return m_molecule->residues.m_begin == m_molecule->residues.m_end;
}
size_t proxy::Molecule::size() const {
  assert(m_molecule);
  assert(m_molecule->frame);
  return m_molecule->residues.m_end - m_molecule->residues.m_begin;
}
proxy::Molecule::Molecule(BaseMolecule& molecule) : m_molecule(&molecule) {}
Frame& proxy::Molecule::frame() noexcept { return *m_molecule->frame; }
proxy::ProxySpan<proxy::Residue, BaseResidue> proxy::Molecule::residues() {
  return proxy::ProxySpan<proxy::Residue, BaseResidue>{m_molecule->residues};
}
proxy::ProxySpan<proxy::Atom, BaseAtom> proxy::Molecule::atoms() {
  if (empty())
    return {};
  assert(m_molecule->residues.m_begin);
  assert(m_molecule->residues.m_end);
  return proxy::ProxySpan<proxy::Atom, BaseAtom>(m_molecule->residues.m_begin->atoms.m_begin,
                                                 (m_molecule->residues.m_begin + size() - 1)->atoms.m_end);
}
proxy::Residue proxy::Molecule::add_residue(const ResidueName& residueName, const ResidueId& residueId) {
  return proxy::Residue(frame().add_residue(*m_molecule, residueName, residueId));
}

const ResidueName& proxy::Residue::name() const {
  assert(m_residue);
  return m_residue->name;
}
void proxy::Residue::name(const ResidueName& name) {
  assert(m_residue);
  m_residue->name = name;
}
bool proxy::Residue::empty() const {
  assert(m_residue);
  return m_residue->atoms.m_begin == m_residue->atoms.m_end;
}
size_t proxy::Residue::size() const {
  assert(m_residue);
  return m_residue->atoms.m_end - m_residue->atoms.m_begin;
}
proxy::Residue::Residue(BaseResidue& residue) : m_residue(&residue) {}
proxy::Molecule proxy::Residue::molecule() noexcept { return proxy::Molecule(*m_residue->molecule); }
Frame& proxy::Residue::frame() noexcept { return *m_residue->molecule->frame; }
proxy::ProxySpan<proxy::Atom, BaseAtom> proxy::Residue::atoms() {
  return proxy::ProxySpan<proxy::Atom, BaseAtom>{m_residue->atoms};
}
proxy::Atom proxy::Residue::add_atom(const AtomName& atomName, const AtomId& atomId) {
  return proxy::Atom(frame().add_atom(*m_residue, atomName, atomId));
}

const AtomId& proxy::Atom::id() const { return m_atom->id; }

proxy::Atom& proxy::Atom::id(const AtomId& value) {
  m_atom->id = value;
  return *this;
}

const AtomName& proxy::Atom::name() const { return m_atom->name; }

proxy::Atom& proxy::Atom::name(const AtomName& value) {
  m_atom->name = value;
  return *this;
}

proxy::Atom::Atom(BaseAtom& atom) : m_coords(&atom.residue->molecule->frame->crd(atom)), m_atom(&atom) {}
proxy::Residue proxy::Atom::residue() noexcept { return proxy::Residue(*m_atom->residue); }
proxy::Atom& proxy::Atom::r(const XYZ& value) {
  *m_coords = value;
  return *this;
}
proxy::Molecule proxy::Atom::molecule() noexcept { return proxy::Molecule(*m_atom->residue->molecule); }
Frame& proxy::Atom::frame() noexcept { return *m_atom->residue->molecule->frame; }

template class proxy::ProxySpan<proxy::Atom, BaseAtom>;
template class proxy::ProxySpan<proxy::Residue, BaseResidue>;
template class proxy::ProxySpan<proxy::Molecule, BaseMolecule>;