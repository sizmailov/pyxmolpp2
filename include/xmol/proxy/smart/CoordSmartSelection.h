#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// CoordSelection with parent frame changes tracking
class CoordSmartSelection : public FrameObserver<CoordSmartSelection> {
public:
  CoordSmartSelection(CoordSelection sel);

  /// Returns selection with Coords that match predicate
  template <typename Predicate> CoordSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_selection.filter(std::forward<Predicate>(p));
  }

  [[nodiscard]] auto begin() {
    check_precondition("begin()");
    return m_selection.begin();
  }
  [[nodiscard]] auto end() {
    check_precondition("end()");
    return m_selection.end();
  }
  [[nodiscard]] size_t size() const {
    check_precondition("size()");
    return m_selection.size();
  }
  [[nodiscard]] size_t empty() const {
    check_precondition("empty()");
    return m_selection.empty();
  }

  CoordRef& operator[](size_t i) {
    check_precondition("operator[]()");
    return m_selection[i];
  }

  operator const CoordSelection&() const {
    check_precondition("operator const CoordSelection&()");
    return m_selection;
  }

  operator CoordSelection&() {
    check_precondition("operator CoordSelection&()");
    return m_selection;
  }

  CoordEigenMatrix _eigen() {
    check_precondition("_eigen()");
    return m_selection._eigen();
  }

  void _eigen(const CoordEigenMatrix& matrix) {
    check_precondition("_eigen()");
    m_selection._eigen(matrix);
  }

private:
  CoordSelection m_selection;
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_selection.empty()) {
      throw DeadFrameAccessError(std::string("CoordSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct CoordRefLessThanComparator;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
};

} // namespace xmol::proxy::smart