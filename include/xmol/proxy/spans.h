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

protected:
  CoordSpan() = default;
  CoordSpan(Frame& frame, future::Span<XYZ>& span) : ProxySpan<CoordRef, XYZ>(span), m_frame(&frame){};
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

  /// Make smart span from this
  smart::AtomSmartSpan smart();

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->residue->molecule->frame; }

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

  /// Make smart span from this
  smart::ResidueSmartSpan smart();

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->molecule->frame; }

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

  /// Make smart span from this
  smart::MoleculeSmartSpan smart();

protected:
  Frame* frame_ptr() { return empty() ? nullptr : m_begin->frame; }

private:
  friend smart::MoleculeSmartSpan;
};

} // namespace xmol::proxy