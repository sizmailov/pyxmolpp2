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
  friend AtomRefSpan;
  friend smart::CoordSmartSpan;
//  friend smart::CoordSmartSelection;
  Frame* m_frame = nullptr;
  friend smart::MoleculeSmartSpan;
};

class AtomRefSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  ResidueRefSpan residues();
  MoleculeRefSpan molecules();


  [[nodiscard]] bool contains(const AtomRef& ref) const;

  template <typename Predicate> AtomSelection filter(Predicate&& p);
  std::vector<AtomIndex> index() const;
  AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  AtomRefSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::AtomSmartSpan smart();

  /// Guess atom mass by atom name
  void guess_mass();

  /// Intersect inplace
  AtomRefSpan& operator&=(const AtomRefSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->residue->molecule->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->residue->molecule->frame; }

private:
  friend smart::AtomSmartSpan;
};

class ResidueRefSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  AtomRefSpan atoms();
  MoleculeRefSpan molecules();

  [[nodiscard]] bool contains(const ResidueRef& ref) const;

  template <typename Predicate> ResidueSelection filter(Predicate&& p);
  std::vector<ResidueIndex> index() const;

  ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  ResidueRefSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::ResidueSmartSpan smart();

  /// Intersect inplace
  ResidueRefSpan& operator&=(const ResidueRefSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->molecule->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->molecule->frame; }

private:
  friend smart::ResidueSmartSpan;
};

class MoleculeRefSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;

  CoordSpan coords();
  AtomRefSpan atoms();
  ResidueRefSpan residues();

  [[nodiscard]] bool contains(const MoleculeRef& ref) const;

  template <typename Predicate> MoleculeSelection filter(Predicate&& p);
  std::vector<MoleculeIndex> index() const;

  MoleculeSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step);
  MoleculeRefSpan slice(std::optional<size_t> start, std::optional<size_t> stop);

  /// Make smart span from this
  smart::MoleculeSmartSpan smart();

  /// Intersect inplace
  MoleculeRefSpan& operator&=(const MoleculeRefSpan& rhs);

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->frame; }

private:
  friend smart::MoleculeSmartSpan;
};

AtomSelection operator|(const AtomRefSpan& lhs, const AtomRefSpan& rhs);
AtomSelection operator-(const AtomRefSpan& lhs, const AtomRefSpan& rhs);
AtomRefSpan operator&(const AtomRefSpan& lhs, const AtomRefSpan& rhs);

ResidueSelection operator|(const ResidueRefSpan& lhs, const ResidueRefSpan& rhs);
ResidueSelection operator-(const ResidueRefSpan& lhs, const ResidueRefSpan& rhs);
ResidueRefSpan operator&(const ResidueRefSpan& lhs, const ResidueRefSpan& rhs);

MoleculeSelection operator|(const MoleculeRefSpan& lhs, const MoleculeRefSpan& rhs);
MoleculeSelection operator-(const MoleculeRefSpan& lhs, const MoleculeRefSpan& rhs);
MoleculeRefSpan operator&(const MoleculeRefSpan& lhs, const MoleculeRefSpan& rhs);

} // namespace xmol::proxy