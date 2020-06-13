
#include <xmol/proxy/spans.h>

#include "xmol/proxy/smart/spans.h"
#include "xmol/algo/alignment.h"
#include "xmol/proxy/selections.h"

using namespace xmol::proxy;

smart::CoordSmartSpan CoordSpan::smart() { return smart::CoordSmartSpan(*this); }

xmol::geom::affine::Transformation3d CoordSpan::alignment_to(CoordSpan& other) {
  return xmol::algo::calc_alignment(other, *this);
}
xmol::geom::affine::Transformation3d CoordSpan::alignment_to(CoordSelection& other) {
  return xmol::algo::calc_alignment(other, *this);
}

double CoordSpan::rmsd(CoordSpan& other) { return xmol::algo::calc_rmsd(other, *this); }
double CoordSpan::rmsd(CoordSelection& other) { return xmol::algo::calc_rmsd(other, *this); }
Eigen::Matrix3d CoordSpan::inertia_tensor() { return xmol::algo::calc_inertia_tensor(*this); }


void CoordSpan::apply(const geom::affine::Transformation3d& t){
  auto map = this->_eigen();
  map = (t.get_underlying_matrix() * map.transpose()).transpose().rowwise() + t.get_translation()._eigen();
}
void CoordSpan::apply(const geom::affine::UniformScale3d& t){
  this->_eigen().array() *= t.scale();
}
void CoordSpan::apply(const geom::affine::Rotation3d& t){
  auto map = this->_eigen();
  map = (t.get_underlying_matrix() * map.transpose()).transpose();
}
void CoordSpan::apply(const geom::affine::Translation3d& t){
  auto map = this->_eigen();
  map = map.rowwise() + t.dr()._eigen();
}

CoordSpan AtomRefSpan::coords() {
  if (empty()) {
    return {};
  }
  return {begin()->frame(), begin()->m_coord, size()};
}

ResidueRefSpan AtomRefSpan::residues() {
  if (empty()) {
    return {};
  }
  return ResidueRefSpan(m_begin->residue, (m_begin + size() - 1)->residue + 1);
}

MoleculeRefSpan AtomRefSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeRefSpan(m_begin->residue->molecule, (m_begin + size() - 1)->residue->molecule + 1);
}

CoordSpan ResidueRefSpan::coords() { return atoms().coords(); }

AtomRefSpan ResidueRefSpan::atoms() {
  if (empty()) {
    return {};
  }
  return AtomRefSpan(m_begin->atoms.m_begin, (m_begin + size() - 1)->atoms.m_end);
}
MoleculeRefSpan ResidueRefSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeRefSpan(m_begin->molecule, (m_begin + size() - 1)->molecule + 1);
}

CoordSpan MoleculeRefSpan::coords() { return atoms().coords(); }

AtomRefSpan MoleculeRefSpan::atoms() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  auto last_mol_last_residue = last_mol->residues.m_begin + last_mol->residues.size() - 1;
  return AtomRefSpan(m_begin->residues.m_begin->atoms.m_begin, last_mol_last_residue->atoms.m_end);
}
ResidueRefSpan MoleculeRefSpan::residues() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  return ResidueRefSpan(m_begin->residues.m_begin, last_mol->residues.m_begin + last_mol->residues.size());
}

bool AtomRefSpan::contains(const AtomRef& ref) const {
  return m_begin <= ref.m_atom && ref.m_atom < m_end;
  ;
}

smart::AtomSmartSpan AtomRefSpan::smart() { return *this; }

bool ResidueRefSpan::contains(const ResidueRef& ref) const { return m_begin <= ref.m_residue && ref.m_residue < m_end; }
smart::ResidueSmartSpan ResidueRefSpan::smart() { return *this; }

bool MoleculeRefSpan::contains(const MoleculeRef& ref) const {
  return m_begin <= ref.m_molecule && ref.m_molecule < m_end;
  ;
}
smart::MoleculeSmartSpan MoleculeRefSpan::smart() { return *this; }
