#include "xmol/v1/proxy/selections.h"

using namespace xmol::v1::proxy;

ResidueSelection AtomSelection::residues() {
  check_precondition("residues()");
  std::vector<ResidueRef> result;
  BaseResidue* prev = nullptr;
  for (auto& a : m_data) {
    if (prev != a.m_atom->residue) {
      result.push_back(ResidueRef{*a.m_atom->residue});
      prev = a.m_atom->residue;
    }
  }
  return ResidueSelection(std::move(result), true);
}
MoleculeSelection AtomSelection::molecules() {
  check_precondition("molecules()");
  std::vector<MoleculeRef> result;
  BaseMolecule* prev = nullptr;
  for (auto& a : m_data) {
    BaseMolecule* mol = a.m_atom->residue->molecule;
    if (prev != mol) {
      result.push_back(MoleculeRef{*mol});
      prev = mol;
    }
  }
  return MoleculeSelection(std::move(result), true);
}
MoleculeSelection ResidueSelection::molecules() {
  check_precondition("molecules()");
  std::vector<MoleculeRef> result;
  BaseMolecule* prev = nullptr;
  for (auto& r : m_data) {
    if (prev != r.m_residue->molecule) {
      result.push_back(MoleculeRef{*r.m_residue->molecule});
      prev = r.m_residue->molecule;
    }
  }
  return MoleculeSelection(std::move(result), true);
}
AtomSelection ResidueSelection::atoms() {
  check_precondition("atoms()");
  std::vector<AtomRef> result;
  for (auto& r : m_data) {
    for (auto a : r.atoms()) {
      result.push_back(a);
    }
  }
  return AtomSelection(std::move(result), true);
}
ResidueSelection MoleculeSelection::residues() {
  check_precondition("residues()");
  std::vector<ResidueRef> result;
  for (auto& m : m_data) {
    for (auto& r : m.residues()) {
      result.push_back(r);
    }
  }
  return ResidueSelection(std::move(result), true);
}
AtomSelection MoleculeSelection::atoms() {
  check_precondition("atoms()");
  std::vector<AtomRef> result;
  for (auto& m : m_data) {
    for (auto& r : m.residues()) {
      for (auto& a: r.atoms()){
        result.push_back(a);
      }
    }
  }
  return AtomSelection(std::move(result), true);
}
