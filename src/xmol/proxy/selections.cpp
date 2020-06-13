
#include "xmol/proxy/selections.h"
#include "xmol/algo/alignment.h"
#include "xmol/proxy/smart/selections.h"

using namespace xmol::proxy;

xmol::geom::affine::Transformation3d CoordSelection::alignment_to(CoordSpan& other) {
  return xmol::algo::calc_alignment(other, *this);
}
xmol::geom::affine::Transformation3d CoordSelection::alignment_to(CoordSelection& other) {
  return xmol::algo::calc_alignment(other, *this);
}

double CoordSelection::rmsd(CoordSpan& other) { return xmol::algo::calc_rmsd(other, *this); }
double CoordSelection::rmsd(CoordSelection& other) { return xmol::algo::calc_rmsd(other, *this); }
Eigen::Matrix3d CoordSelection::inertia_tensor() { return xmol::algo::calc_inertia_tensor(*this); }

xmol::CoordEigenMatrix CoordSelection::_eigen() {
  CoordEigenMatrix matrix(size(), 3);
  size_t i = 0;
  for (auto& x : *this) {
    matrix(i++, Eigen::all) = x._eigen();
  }
  return matrix;
}

void CoordSelection::_eigen(const CoordEigenMatrix& matrix) {
  if (size() != matrix.outerSize()) {
    throw CoordSelectionSizeMismatchError("Selection size must match matrix");
  }
  size_t i = 0;
  for (auto& x : *this) {
    x._eigen() = matrix(i++, Eigen::all);
  }
}

void CoordSelection::apply(const geom::affine::Transformation3d& t) {
  auto map = this->_eigen();
  this->_eigen((t.get_underlying_matrix() * map.transpose()).transpose().rowwise() + t.get_translation()._eigen());
}
void CoordSelection::apply(const geom::affine::UniformScale3d& t) { this->_eigen(this->_eigen().array() * t.scale()); }
void CoordSelection::apply(const geom::affine::Rotation3d& t) {
  this->_eigen((t.get_underlying_matrix() * this->_eigen().transpose()).transpose());
}
void CoordSelection::apply(const geom::affine::Translation3d& t) {
  this->_eigen(this->_eigen().rowwise() + t.dr()._eigen());
}
xmol::XYZ CoordSelection::mean() {
  XYZ result;
  for (auto& r : *this) {
    result += r;
  }
  return result;
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