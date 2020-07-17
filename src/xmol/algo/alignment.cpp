#include "xmol/algo/alignment.h"
#include "xmol/algo/alignment-impl.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/spans.h"

using namespace xmol::algo;
using namespace xmol::geom::affine;

namespace {

template <typename AtomsA, typename AtomsB> double calc_weighted_rmsd_atoms_impl(AtomsA& reference, AtomsB& variable) {
  if (reference.size() != variable.size()) {
    throw xmol::geom::GeomError("can't calc rmsd on atom selections of different size");
  }
  double displacement = 0;
  double mass = 0;
  auto it2 = variable.begin();
  bool mass_mismatch = false;
  for (auto&& a : reference) {
    displacement += a.r().distance2(it2->r()) * a.mass();
    mass += a.mass();
    mass_mismatch |= a.mass() != (it2->mass());
    ++it2;
  }
  if (mass_mismatch) {
    throw xmol::geom::GeomError("Mass of atoms is different."
                                "If you want ignore mass use rmsd of coordinates instead.");
  }
  return ::sqrt(displacement / mass);
}

template <typename AtomsA, typename AtomsB>
Transformation3d calc_alignment_atoms_impl(AtomsA& reference, AtomsB& variable) {
  if (reference.size() != variable.size()) {
    throw xmol::geom::GeomError("can't align atom selections of different size");
  }
  std::vector<double> mass;
  mass.reserve(reference.size());

  auto it2 = variable.begin();
  bool mass_mismatch = false;
  for (auto&& x : reference) {
    mass.push_back(x.mass());
    mass_mismatch |= x.mass() != (it2->mass());
    ++it2;
  }

  if (mass_mismatch) {
    throw xmol::geom::GeomError("Mass of reference atoms doesn't match mass aligned ones."
                                "If you want ignore mass use alignment of coordinates instead.");
  }
  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass_map(mass.data(), mass.size(), 1);
  return calc_alignment_weighted_impl(reference.coords()._eigen(), variable.coords()._eigen(), mass_map);
}

template <typename Atoms> Eigen::Matrix3d calc_intertia_tensor_atoms_impl(Atoms& reference) {
  std::vector<double> mass;
  mass.reserve(reference.size());
  for (auto&& x : reference) {
    mass.push_back(x.mass());
  }
  auto mass_span = xmol::future::Span(mass);
  auto&& coords = reference.coords();
  return calc_inertia_tensor(coords, mass_span);
}

} // namespace

Transformation3d xmol::algo::calc_alignment(const proxy::CoordSpan& reference, const proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(const proxy::CoordSpan& reference, const proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(const proxy::CoordSelection& reference, const proxy::CoordSpan& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(const proxy::CoordSelection& reference,
                                            const proxy::CoordSelection& variable) {
  return calc_alignment_impl(reference._eigen(), variable._eigen());
}
Transformation3d xmol::algo::calc_alignment(const proxy::AtomSpan& reference, const proxy::AtomSpan& variable) {
  return calc_alignment_atoms_impl(reference, variable);
}
Transformation3d xmol::algo::calc_alignment(const xmol::proxy::AtomSpan& reference,
                                            const xmol::proxy::AtomSelection& variable) {
  return calc_alignment_atoms_impl(reference, variable);
}
Transformation3d xmol::algo::calc_alignment(const xmol::proxy::AtomSelection& reference,
                                            const xmol::proxy::AtomSpan& variable) {
  return calc_alignment_atoms_impl(reference, variable);
}
Transformation3d xmol::algo::calc_alignment(const xmol::proxy::AtomSelection& reference,
                                            const xmol::proxy::AtomSelection& variable) {
  return calc_alignment_atoms_impl(reference, variable);
}

double xmol::algo::calc_rmsd(const proxy::CoordSpan& reference, const proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(const proxy::CoordSpan& reference, const proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(const proxy::CoordSelection& reference, const proxy::CoordSpan& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}
double xmol::algo::calc_rmsd(const proxy::CoordSelection& reference, const proxy::CoordSelection& variable) {
  return calc_rmsd_impl(reference._eigen(), variable._eigen());
}

double xmol::algo::calc_weighted_rmsd(const proxy::AtomSpan& reference, const proxy::AtomSpan& variable) {
  return calc_weighted_rmsd_atoms_impl(reference, variable);
}
double xmol::algo::calc_weighted_rmsd(const proxy::AtomSpan& reference, const proxy::AtomSelection& variable) {
  return calc_weighted_rmsd_atoms_impl(reference, variable);
}
double xmol::algo::calc_weighted_rmsd(const proxy::AtomSelection& reference, const proxy::AtomSpan& variable) {
  return calc_weighted_rmsd_atoms_impl(reference, variable);
}
double xmol::algo::calc_weighted_rmsd(const proxy::AtomSelection& reference, const proxy::AtomSelection& variable) {
  return calc_weighted_rmsd_atoms_impl(reference, variable);
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::CoordSelection& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}
Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::CoordSpan& coords) {
  return calc_inertia_tensor_impl(coords._eigen());
}
Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::CoordSelection& coords,
                                                const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass_map(mass.m_begin, mass.size(), 1);
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::CoordSpan& coords,
                                                const future::Span<double>& mass) {
  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass_map(mass.m_begin, mass.size(), 1);
  return calc_inertia_tensor_impl(coords._eigen(), mass_map);
}

Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::AtomSelection& atoms) {
  return calc_intertia_tensor_atoms_impl(atoms);
}
Eigen::Matrix3d xmol::algo::calc_inertia_tensor(const xmol::proxy::AtomSpan& atoms) {
  return calc_intertia_tensor_atoms_impl(atoms);
}
