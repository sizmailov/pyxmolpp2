#include "xmol/v1/proxies.h"
#include "xmol/v1/Frame.h"

using namespace xmol::v1;

MoleculeName proxy::Molecule::name() const {
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
Frame& proxy::Molecule::frame() noexcept {return *m_molecule->frame;}

ResidueName proxy::Residue::name() const {
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

proxy::Atom::Atom(BaseAtom& atom, XYZ& coords) : m_coords(&coords), m_atom(&atom) {}
proxy::Residue proxy::Atom::residue() noexcept { return proxy::Residue(*m_atom->residue); }
proxy::Atom& proxy::Atom::r(const XYZ& value) {
  *m_coords = value;
  return *this;
}
proxy::Molecule proxy::Atom::molecule() noexcept { return proxy::Molecule(*m_atom->residue->molecule); }
Frame& proxy::Atom::frame() noexcept { return *m_atom->residue->molecule->frame; }
