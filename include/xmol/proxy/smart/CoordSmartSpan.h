#pragma once
#include "../spans-impl.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// CoordSpan with parent frame changes tracking
class CoordSmartSpan : public FrameObserver<CoordSmartSpan> {
public:
  CoordSmartSpan(CoordSpan span);

  /// Returns selection with Coords that match predicate
  template <typename Predicate> CoordSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_span.filter(std::forward<Predicate>(p));
  }

  geom::affine::Transformation3d alignment_to(CoordSpan& other);
  geom::affine::Transformation3d alignment_to(CoordSelection& other);

  double rmsd(CoordSpan& other) {
    check_precondition("rmsd()");
    return m_span.rmsd(other);
  }

  double rmsd(CoordSelection& other) {
    check_precondition("rmsd()");
    return m_span.rmsd(other);
  }

  Eigen::Matrix3d inertia_tensor() {
    check_precondition("inertia_tensor()");
    return m_span.inertia_tensor();
  }

  [[nodiscard]] auto begin() {
    check_precondition("begin()");
    return m_span.begin();
  }
  [[nodiscard]] auto end() {
    check_precondition("end()");
    return m_span.end();
  }
  [[nodiscard]] size_t size() const {
    check_precondition("size()");
    return m_span.size();
  }
  [[nodiscard]] size_t empty() const {
    check_precondition("empty()");
    return m_span.empty();
  }

  CoordRef operator[](size_t i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator const CoordSpan&() const & {
    check_precondition("operator CoordSpan()");
    return m_span;
  }

  operator CoordSpan&() & {
    check_precondition("operator CoordSpan()");
    return m_span;
  }

  operator CoordSpan&&() && {
    check_precondition("operator CoordSpan()");
    return std::move(m_span);
  }

  Eigen::Map<CoordEigenMatrix> _eigen() {
    check_precondition("_eigen()");
    return m_span._eigen();
  }

  void apply(const geom::affine::Transformation3d& t) {
    check_precondition("apply()");
    m_span.apply(t);
  }
  void apply(const geom::affine::UniformScale3d& t) {
    check_precondition("apply()");
    m_span.apply(t);
  }
  void apply(const geom::affine::Rotation3d& t) {
    check_precondition("apply()");
    m_span.apply(t);
  }
  void apply(const geom::affine::Translation3d& t) {
    check_precondition("apply()");
    m_span.apply(t);
  }
  geom::XYZ mean() {
    check_precondition("mean()");
    return m_span.mean();
  }

  CoordSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) {
    check_precondition("slice()");
    return m_span.slice(start, stop, step);
  }

  CoordSelection slice(std::optional<size_t> start, std::optional<size_t> stop) {
    check_precondition("slice()");
    return m_span.slice(start, stop);
  }

  auto index() {
    check_precondition("index()");
    return m_span.index();
  }

  Frame& frame() {
    check_precondition("frame()");
    return FrameObserver::frame();
  }

private:
  CoordSpan m_span;
  bool m_is_split = false; /// indicates invalid span state
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_span.empty()) {
      throw DeadFrameAccessError(std::string("CoordSmartSpan::") + func_name);
    }
    if (m_is_split) {
      throw SpanSplitError(std::string("CoordSmartSpan::") + func_name);
    }
  }
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
};

} // namespace xmol::proxy::smart