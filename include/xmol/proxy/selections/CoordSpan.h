#pragma once

#include "xmol/proxy/ProxySpan.h"
#include "xmol/base.h"
#include "xmol/geom/fwd.h"

namespace xmol::proxy{

class CoordSpan : public ProxySpan<CoordRef, XYZ> {
public:
  [[nodiscard]] Eigen::Map<CoordEigenMatrix> _eigen()const  {
    return Eigen::Map<CoordEigenMatrix>(empty() ? nullptr : m_begin->_eigen().data(), size(), 3);
  }

  template <typename Predicate> [[nodiscard]] CoordSelection filter(Predicate&& p) const;
  [[nodiscard]] std::vector<CoordIndex> index() const;
  [[nodiscard]] CoordSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) const;
  [[nodiscard]] CoordSpan slice(std::optional<size_t> start, std::optional<size_t> stop) const;

  [[nodiscard]] smart::CoordSmartSpan smart() const;

  [[nodiscard]] geom::affine::Transformation3d alignment_to(CoordSpan& other) const;
  [[nodiscard]] geom::affine::Transformation3d alignment_to(CoordSelection& other) const;

  [[nodiscard]] double rmsd(CoordSpan& other) const;
  [[nodiscard]] double rmsd(CoordSelection& other) const;

  [[nodiscard]] Eigen::Matrix3d inertia_tensor() const;

  void apply(const geom::affine::Transformation3d& t) const;
  void apply(const geom::affine::UniformScale3d& t) const;
  void apply(const geom::affine::Rotation3d& t) const;
  void apply(const geom::affine::Translation3d& t) const;

  [[nodiscard]] XYZ mean() const;

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

}