#pragma once
#include "ProxySpan.h"
#include "xmol/base.h"
#include "xmol/geom/fwd.h"

namespace xmol::proxy {

class CoordSpan : public ProxySpan<CoordRef, XYZ> {
public:
  Eigen::Map<CoordEigenMatrix> _eigen() {
    return Eigen::Map<CoordEigenMatrix>(empty() ? nullptr : m_begin->_eigen().data(), size(), 3);
  }

  template <typename Predicate> CoordSelection filter(Predicate&& p);
  std::vector<CoordIndex> index() const;
  CoordSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  CoordSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  smart::CoordSmartSpan smart();

  geom::affine::Transformation3d alignment_to(CoordSpan& other);
  geom::affine::Transformation3d alignment_to(CoordSelection& other);

  double rmsd(CoordSpan& other);
  double rmsd(CoordSelection& other);

  Eigen::Matrix3d inertia_tensor();

  void apply(const geom::affine::Transformation3d& t);
  void apply(const geom::affine::UniformScale3d& t);
  void apply(const geom::affine::Rotation3d& t);
  void apply(const geom::affine::Translation3d& t);

  XYZ mean();

protected:
  CoordSpan() = default;
  CoordSpan(Frame& frame, future::Span<XYZ> span) : ProxySpan<CoordRef, XYZ>(span), m_frame(&frame){};
  CoordSpan(Frame& frame, XYZ* b, XYZ* e) : ProxySpan(b, e), m_frame(&frame){};
  CoordSpan(Frame& frame, XYZ* b, size_t n) : ProxySpan<CoordRef, XYZ>(b, n), m_frame(&frame){};

private:
  friend Frame;
  friend CoordSelection;
  friend AtomSpan;
  friend smart::CoordSmartSpan;
//  friend smart::CoordSmartSelection;
  Frame* m_frame = nullptr;
  friend smart::MoleculeSmartSpan;
};

class AtomSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  ResidueSpan residues();
  MoleculeSpan molecules();


  [[nodiscard]] bool contains(const AtomRef& ref) const;

  template <typename Predicate> AtomSelection filter(Predicate&& p);
  std::vector<AtomIndex> index() const;
  AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  AtomSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::AtomSmartSpan smart();

  /// Guess atom mass by atom name
  void guess_mass();

  /// Guess atom mass by atom name
  [[nodiscard]] Eigen::Matrix3d inertia_tensor();

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSpan& rhs, bool weighted=false);

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSelection& rhs, bool weighted=false);

  [[nodiscard]] double rmsd(AtomSelection& rhs, bool weighted=false);
  [[nodiscard]] double rmsd(AtomSpan& rhs, bool weighted=false);

  [[nodiscard]] XYZ mean(bool weighted=false);

  /// Intersect inplace
  AtomSpan& operator&=(const AtomSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->residue->molecule->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->residue->molecule->frame; }

private:
  friend smart::AtomSmartSpan;
};

class ResidueSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  AtomSpan atoms();
  MoleculeSpan molecules();

  [[nodiscard]] bool contains(const ResidueRef& ref) const;

  template <typename Predicate> ResidueSelection filter(Predicate&& p);
  std::vector<ResidueIndex> index() const;

  ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  ResidueSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::ResidueSmartSpan smart();

  /// Intersect inplace
  ResidueSpan& operator&=(const ResidueSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->molecule->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->molecule->frame; }

private:
  friend smart::ResidueSmartSpan;
};

class MoleculeSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  AtomSpan atoms();
  ResidueSpan residues();

  [[nodiscard]] bool contains(const MoleculeRef& ref) const;

  template <typename Predicate> MoleculeSelection filter(Predicate&& p);
  std::vector<MoleculeIndex> index() const;

  MoleculeSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  MoleculeSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::MoleculeSmartSpan smart();

  /// Intersect inplace
  MoleculeSpan& operator&=(const MoleculeSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->frame; }

private:
  friend smart::MoleculeSmartSpan;
};

AtomSelection operator|(const AtomSpan& lhs, const AtomSpan& rhs);
AtomSelection operator-(const AtomSpan& lhs, const AtomSpan& rhs);
AtomSpan operator&(const AtomSpan& lhs, const AtomSpan& rhs);

ResidueSelection operator|(const ResidueSpan& lhs, const ResidueSpan& rhs);
ResidueSelection operator-(const ResidueSpan& lhs, const ResidueSpan& rhs);
ResidueSpan operator&(const ResidueSpan& lhs, const ResidueSpan& rhs);

MoleculeSelection operator|(const MoleculeSpan& lhs, const MoleculeSpan& rhs);
MoleculeSelection operator-(const MoleculeSpan& lhs, const MoleculeSpan& rhs);
MoleculeSpan operator&(const MoleculeSpan& lhs, const MoleculeSpan& rhs);

} // namespace xmol::proxy