
#include "xmol/proxy/selections.h"
#include "xmol/proxy/smart/selections.h"

using namespace xmol::proxy;

xmol::CoordEigenMatrix CoordSelection::_eigen() {
  CoordEigenMatrix matrix(3, size());
  size_t i = 0;
  for (auto& x : *this) {
    matrix(Eigen::all, i++) = x._eigen();
  }
  return matrix;
}

void CoordSelection::_eigen(const CoordEigenMatrix& matrix) {
  if (size()!=matrix.outerSize()){
    throw CoordSelectionSizeMismatchError("Selection size must match matrix");
  }
  size_t i=0;
  for (auto&x: *this){
    x._eigen() = matrix(Eigen::all, i++);
  }
}

CoordSelection AtomSelection::coords() {
  std::vector<CoordRef> result;
  for (auto& a : m_data) {
    result.push_back(CoordRef(*a.m_coord));
  }
  return CoordSelection(*frame_ptr(), std::move(result), true);
}
ResidueSelection AtomSelection::residues() {
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
CoordSelection ResidueSelection::coords() {
  std::vector<CoordRef> result;
  for (auto& r : m_data) {
    for (auto a : r.atoms()) {
      result.push_back(CoordRef(*a.m_coord));
    }
  }
  return CoordSelection(*frame_ptr(), std::move(result), true);
}
AtomSelection ResidueSelection::atoms() {
  std::vector<AtomRef> result;
  for (auto& r : m_data) {
    for (auto a : r.atoms()) {
      result.push_back(a);
    }
  }
  return AtomSelection(std::move(result), true);
}
ResidueSelection MoleculeSelection::residues() {
  std::vector<ResidueRef> result;
  for (auto& m : m_data) {
    for (auto& r : m.residues()) {
      result.push_back(r);
    }
  }
  return ResidueSelection(std::move(result), true);
}

CoordSelection MoleculeSelection::coords() {
  std::vector<CoordRef> result;
  for (auto& m : m_data) {
    for (auto& r : m.residues()) {
      for (auto& a : r.atoms()) {
        result.push_back(CoordRef(*a.m_coord));
      }
    }
  }
  return CoordSelection(*frame_ptr(), std::move(result), true);
}
AtomSelection MoleculeSelection::atoms() {
  std::vector<AtomRef> result;
  for (auto& m : m_data) {
    for (auto& r : m.residues()) {
      for (auto& a : r.atoms()) {
        result.push_back(a);
      }
    }
  }
  return AtomSelection(std::move(result), true);
}

smart::MoleculeSmartSelection MoleculeSelection::smart() { return smart::MoleculeSmartSelection(*this); }
smart::ResidueSmartSelection ResidueSelection::smart() { return smart::ResidueSmartSelection(*this); }
smart::AtomSmartSelection AtomSelection::smart() { return smart::AtomSmartSelection(*this); }

namespace xmol::proxy {

AtomSelection operator|(const AtomSelection& lhs, const AtomSelection& rhs) { return AtomSelection(lhs) |= rhs; }
AtomSelection operator-(const AtomSelection& lhs, const AtomSelection& rhs) { return AtomSelection(lhs) -= rhs; }
AtomSelection operator&(const AtomSelection& lhs, const AtomSelection& rhs) { return AtomSelection(lhs) &= rhs; }

ResidueSelection operator|(const ResidueSelection& lhs, const ResidueSelection& rhs) {
  return ResidueSelection(lhs) |= rhs;
}
ResidueSelection operator-(const ResidueSelection& lhs, const ResidueSelection& rhs) {
  return ResidueSelection(lhs) -= rhs;
}
ResidueSelection operator&(const ResidueSelection& lhs, const ResidueSelection& rhs) {
  return ResidueSelection(lhs) &= rhs;
}

MoleculeSelection operator|(const MoleculeSelection& lhs, const MoleculeSelection& rhs) {
  return MoleculeSelection(lhs) |= rhs;
}
MoleculeSelection operator-(const MoleculeSelection& lhs, const MoleculeSelection& rhs) {
  return MoleculeSelection(lhs) -= rhs;
}
MoleculeSelection operator&(const MoleculeSelection& lhs, const MoleculeSelection& rhs) {
  return MoleculeSelection(lhs) &= rhs;
}

smart::CoordSmartSelection CoordSelection::smart() { return smart::CoordSmartSelection(*this); }
} // namespace xmol::proxy