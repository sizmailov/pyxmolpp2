#include "xmol/v1/proxy/spans.h"

xmol::v1::proxy::ResidueRefSpan xmol::v1::proxy::AtomRefSpan::residues() {
  if (empty()) {
    return {};
  }
  return ResidueRefSpan(m_begin->residue, (m_begin + size() - 1)->residue + 1);
}

xmol::v1::proxy::MoleculeRefSpan xmol::v1::proxy::AtomRefSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeRefSpan(m_begin->residue->molecule, (m_begin + size() - 1)->residue->molecule + 1);
}
xmol::v1::proxy::AtomRefSpan xmol::v1::proxy::ResidueRefSpan::atoms() {
  if (empty()) {
    return {};
  }
  return AtomRefSpan(m_begin->atoms.m_begin, (m_begin + size() - 1)->atoms.m_end);
}
xmol::v1::proxy::MoleculeRefSpan xmol::v1::proxy::ResidueRefSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeRefSpan(m_begin->molecule, (m_begin + size() - 1)->molecule + 1);
}
xmol::v1::proxy::AtomRefSpan xmol::v1::proxy::MoleculeRefSpan::atoms() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  auto last_mol_last_residue = last_mol->residues.m_begin + last_mol->residues.size() - 1;
  return xmol::v1::proxy::AtomRefSpan(m_begin->residues.m_begin->atoms.m_begin, last_mol_last_residue->atoms.m_end);
}
xmol::v1::proxy::ResidueRefSpan xmol::v1::proxy::MoleculeRefSpan::residues() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  return xmol::v1::proxy::ResidueRefSpan(m_begin->residues.m_begin,
                                         last_mol->residues.m_begin + last_mol->residues.size());
}
