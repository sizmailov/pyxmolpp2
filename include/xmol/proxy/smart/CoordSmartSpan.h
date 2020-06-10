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

  operator CoordSpan& () {
    check_precondition("operator CoordRefSpan()");
    return m_span;
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