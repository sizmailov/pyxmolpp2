#pragma once

#include "CoordSpan.h"

namespace xmol::proxy {

class AtomSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;

  [[nodiscard]] CoordSpan coords() const;
  [[nodiscard]] ResidueSpan residues() const;
  [[nodiscard]] MoleculeSpan molecules() const;

  [[nodiscard]] bool contains(const AtomRef& ref) const;

  template <typename Predicate> AtomSelection filter(Predicate&& p) const;
  std::vector<AtomIndex> index() const;
  [[nodiscard]] AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) const;
  [[nodiscard]] AtomSpan slice(std::optional<size_t> start, std::optional<size_t> stop) const;

  /// Make smart span from this
  [[nodiscard]] smart::AtomSmartSpan smart() const;

  /// Guess atom mass by atom name
  void guess_mass() const;

  /// Guess atom mass by atom name
  [[nodiscard]] Eigen::Matrix3d inertia_tensor() const;

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSpan& rhs, bool weighted = false);

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSelection& rhs, bool weighted = false);

  [[nodiscard]] double rmsd(AtomSelection& rhs, bool weighted = false);
  [[nodiscard]] double rmsd(AtomSpan& rhs, bool weighted = false);

  /// Intersect inplace
  AtomSpan& operator&=(const AtomSpan& rhs);

protected:
  [[nodiscard]] Frame* frame_ptr() const { return empty() ? nullptr : m_begin->residue->molecule->frame; }

private:
  friend smart::AtomSmartSpan;
};

} // namespace xmol::proxy