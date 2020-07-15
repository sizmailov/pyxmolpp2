
#include "xmol/proxy/spans.h"
#include "xmol/Frame.h"
#include "xmol/algo/alignment.h"
#include "xmol/algo/heuristic/guess_mass.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/smart/spans.h"

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

void CoordSpan::apply(const geom::affine::Transformation3d& t) {
  auto map = this->_eigen();
  map = (t.get_underlying_matrix() * map.transpose()).transpose().rowwise() + t.get_translation()._eigen();
}
void CoordSpan::apply(const geom::affine::UniformScale3d& t) { this->_eigen().array() *= t.scale(); }
void CoordSpan::apply(const geom::affine::Rotation3d& t) {
  auto map = this->_eigen();
  map = (t.get_underlying_matrix() * map.transpose()).transpose();
}
void CoordSpan::apply(const geom::affine::Translation3d& t) {
  auto map = this->_eigen();
  map = map.rowwise() + t.dr()._eigen();
}

xmol::geom::XYZ CoordSpan::mean() { return XYZ(_eigen().colwise().mean()); }
CoordSelection CoordSpan::slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) {
  if (empty()) {
    return {};
  }
  return CoordSelection(*m_frame, slice_impl(start, stop, step), true);
}

CoordSpan CoordSpan::slice(std::optional<size_t> start, std::optional<size_t> stop) {
  if (empty()) {
    return {};
  }
  return CoordSpan(*m_frame, slice_impl(start, stop));
}

std::vector<xmol::CoordIndex> CoordSpan::index() const {
  std::vector<AtomIndex> result;
  if (!empty()) {
    result.reserve(size());
    AtomIndex first = m_frame->index_of(*m_begin);
    for (int i = 0; i < size(); ++i) {
      result.push_back(first + i);
    }
  }
  return result;
}

CoordSpan AtomSpan::coords() {
  if (empty()) {
    return {};
  }
  return {begin()->frame(), begin()->m_coord, size()};
}

ResidueSpan AtomSpan::residues() {
  if (empty()) {
    return {};
  }
  return ResidueSpan(m_begin->residue, (m_begin + size() - 1)->residue + 1);
}

MoleculeSpan AtomSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeSpan(m_begin->residue->molecule, (m_begin + size() - 1)->residue->molecule + 1);
}

CoordSpan ResidueSpan::coords() { return atoms().coords(); }

AtomSpan ResidueSpan::atoms() {
  if (empty()) {
    return {};
  }
  return AtomSpan(m_begin->atoms.m_begin, (m_begin + size() - 1)->atoms.m_end);
}
MoleculeSpan ResidueSpan::molecules() {
  if (empty()) {
    return {};
  }
  return MoleculeSpan(m_begin->molecule, (m_begin + size() - 1)->molecule + 1);
}

CoordSpan MoleculeSpan::coords() { return atoms().coords(); }

AtomSpan MoleculeSpan::atoms() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  auto last_mol_last_residue = last_mol->residues.m_begin + last_mol->residues.size() - 1;
  return AtomSpan(m_begin->residues.m_begin->atoms.m_begin, last_mol_last_residue->atoms.m_end);
}
ResidueSpan MoleculeSpan::residues() {
  if (empty()) {
    return {};
  }
  auto last_mol = m_begin + size() - 1;
  return ResidueSpan(m_begin->residues.m_begin, last_mol->residues.m_begin + last_mol->residues.size());
}

bool AtomSpan::contains(const AtomRef& ref) const {
  return m_begin <= ref.m_atom && ref.m_atom < m_end;
  ;
}

smart::AtomSmartSpan AtomSpan::smart() { return *this; }

AtomSpan& AtomSpan::operator&=(const AtomSpan& rhs) {
  intersect(rhs);
  return *this;
}

AtomSelection AtomSpan::slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) {
  return AtomSelection(slice_impl(start, stop, step), true);
}

AtomSpan AtomSpan::slice(std::optional<size_t> start, std::optional<size_t> stop) {
  return AtomSpan(slice_impl(start, stop));
}

std::vector<xmol::AtomIndex> AtomSpan::index() const {
  std::vector<AtomIndex> result;
  if (!empty()) {
    result.reserve(size());
    AtomIndex first = frame_ptr()->index_of(*m_begin);
    for (int i = 0; i < size(); ++i) {
      result.push_back(first + i);
    }
  }
  return result;
}

void AtomSpan::guess_mass() { algo::heuristic::guess_mass(*this); }

Eigen::Matrix3d AtomSpan::inertia_tensor() { return algo::calc_inertia_tensor(*this); }

[[nodiscard]] xmol::geom::affine::Transformation3d AtomSpan::alignment_to(AtomSpan& rhs, bool weighted){
  if (weighted){
    return algo::calc_alignment(rhs, *this);
  }else{
    auto lhs_coords = this->coords();
    auto rhs_coords = rhs.coords();
    return algo::calc_alignment(rhs_coords, lhs_coords);
  }
}

[[nodiscard]] xmol::geom::affine::Transformation3d AtomSpan::alignment_to(AtomSelection& rhs, bool weighted) {
  if (weighted) {
    return algo::calc_alignment(rhs, *this);
  } else {
    auto lhs_coords = this->coords();
    auto rhs_coords = rhs.coords();
    return algo::calc_alignment(rhs_coords, lhs_coords);
  }
}

[[nodiscard]] double AtomSpan::rmsd(AtomSelection& rhs, bool weighted) {
  if (weighted) {
    return algo::calc_weighted_rmsd(rhs, *this);
  } else {
    auto lhs_coords = this->coords();
    auto rhs_coords = rhs.coords();
    return algo::calc_rmsd(rhs_coords, lhs_coords);
  }
}

[[nodiscard]] double AtomSpan::rmsd(AtomSpan& rhs, bool weighted) {
  if (weighted) {
    return algo::calc_weighted_rmsd(rhs, *this);
  } else {
    auto lhs_coords = this->coords();
    auto rhs_coords = rhs.coords();
    return algo::calc_rmsd(rhs_coords, lhs_coords);
  }
}

[[nodiscard]] xmol::XYZ AtomSpan::mean(bool weighted) {
  if (weighted) {
    double mass = 0;
    XYZ sum{};
    for (auto& a : *this) {
      sum += a.r() * a.mass();
      mass += a.mass();
    }
    return sum / mass;
  } else {
    CoordSpan span = this->coords();
    return span.mean();
  }
}

bool ResidueSpan::contains(const ResidueRef& ref) const { return m_begin <= ref.m_residue && ref.m_residue < m_end; }
smart::ResidueSmartSpan ResidueSpan::smart() { return *this; }

ResidueSpan& ResidueSpan::operator&=(const ResidueSpan& rhs) {
  intersect(rhs);
  return *this;
}

ResidueSelection ResidueSpan::slice(std::optional<size_t> start, std::optional<size_t> stop,
                                    std::optional<size_t> step) {
  return ResidueSelection(slice_impl(start, stop, step), true);
}

ResidueSpan ResidueSpan::slice(std::optional<size_t> start, std::optional<size_t> stop) {
  return ResidueSpan(slice_impl(start, stop));
}
std::vector<xmol::ResidueIndex> ResidueSpan::index() const {
  std::vector<ResidueIndex> result;
  if (!empty()) {
    result.reserve(size());
    ResidueIndex first = frame_ptr()->index_of(*m_begin);
    for (int i = 0; i < size(); ++i) {
      result.push_back(first + i);
    }
  }
  return result;
}

bool MoleculeSpan::contains(const MoleculeRef& ref) const {
  return m_begin <= ref.m_molecule && ref.m_molecule < m_end;
}
smart::MoleculeSmartSpan MoleculeSpan::smart() { return *this; }

MoleculeSpan& MoleculeSpan::operator&=(const MoleculeSpan& rhs) {
  intersect(rhs);
  return *this;
}
MoleculeSelection MoleculeSpan::slice(std::optional<size_t> start, std::optional<size_t> stop,
                                         std::optional<size_t> step) {
  return MoleculeSelection(slice_impl(start, stop, step), true);
}

MoleculeSpan MoleculeSpan::slice(std::optional<size_t> start, std::optional<size_t> stop) {
  return MoleculeSpan(slice_impl(start, stop));
}
std::vector<xmol::MoleculeIndex> MoleculeSpan::index() const {
  std::vector<MoleculeIndex> result;
  if (!empty()) {
    result.reserve(size());
    MoleculeIndex first = frame_ptr()->index_of(*m_begin);
    for (int i = 0; i < size(); ++i) {
      result.push_back(first + i);
    }
  }
  return result;
}

namespace xmol::proxy {

AtomSelection operator|(const AtomSpan& lhs, const AtomSpan& rhs) {
  return AtomSelection(lhs) | AtomSelection(rhs);
}
AtomSelection operator-(const AtomSpan& lhs, const AtomSpan& rhs) {
  return AtomSelection(lhs) - AtomSelection(rhs);
}
AtomSpan operator&(const AtomSpan& lhs, const AtomSpan& rhs) {
  AtomSpan result(lhs);
  result &= rhs;
  return result;
}
ResidueSelection operator|(const ResidueSpan& lhs, const ResidueSpan& rhs) {
  return ResidueSelection(lhs) | ResidueSelection(rhs);
}
ResidueSelection operator-(const ResidueSpan& lhs, const ResidueSpan& rhs) {
  return ResidueSelection(lhs) - ResidueSelection(rhs);
}
ResidueSpan operator&(const ResidueSpan& lhs, const ResidueSpan& rhs) {
  ResidueSpan result(lhs);
  result &= rhs;
  return result;
}

MoleculeSelection operator|(const MoleculeSpan& lhs, const MoleculeSpan& rhs) {
  return MoleculeSelection(lhs) | MoleculeSelection(rhs);
}
MoleculeSelection operator-(const MoleculeSpan& lhs, const MoleculeSpan& rhs) {
  return MoleculeSelection(lhs) - MoleculeSelection(rhs);
}
MoleculeSpan operator&(const MoleculeSpan& lhs, const MoleculeSpan& rhs) {
  MoleculeSpan result(lhs);
  result &= rhs;
  return result;
}

} // namespace xmol::proxy
