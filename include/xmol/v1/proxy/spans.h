#pragma once
#include "../base.h"
#include "ProxySpan.h"

namespace xmol::v1::proxy {

class CoordSpan : public ProxySpan<CoordRef, XYZ> {
public:
  using Matrix_t = Eigen::Matrix<double, 3, Eigen::Dynamic>;
  auto _eigen() { return Eigen::Map<Matrix_t>(empty() ? nullptr : m_begin->_eigen().data(), 3, size()); }

  template <typename Predicate> CoordSelection filter(Predicate&& p);

protected:
  CoordSpan(Frame& frame, future::Span<XYZ>& span) : ProxySpan<CoordRef, XYZ>(span), m_frame(&frame){};
  CoordSpan(Frame& frame, XYZ* b, XYZ* e) : ProxySpan(b, e), m_frame(&frame){};
  CoordSpan(Frame& frame, XYZ* b, size_t n) : ProxySpan<CoordRef, XYZ>(b, n), m_frame(&frame){};

private:
  friend Frame;
  friend CoordSelection;
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

} // namespace xmol::v1::proxy