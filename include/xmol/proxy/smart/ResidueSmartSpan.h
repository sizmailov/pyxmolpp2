#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// ResidueSpan with parent frame changes tracking
class ResidueSmartSpan : public FrameObserver<ResidueSmartSpan> {
public:
  ResidueSmartSpan(ResidueSpan span);

  /// Coordinates
  CoordSpan coords() {
    check_precondition("coords()");
    return m_span.coords();
  };

  /// Child atoms
  AtomSpan atoms() {
    check_precondition("atoms()");
    return m_span.atoms();
  };

  /// Parent molecules
  MoleculeSpan molecules(){
    check_precondition("molecules()");
    return m_span.molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> ResidueSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_span.filter(std::forward<Predicate>(p));
  }

  /// Check if element in selection
  [[nodiscard]] bool contains(const ResidueRef& ref) const{
    check_precondition("filter()");
    return m_span.contains(ref);
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

  ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) {
    check_precondition("slice()");
    return m_span.slice(start, stop, step);
  }

  ResidueSpan slice(std::optional<size_t> start, std::optional<size_t> stop) {
    check_precondition("slice()");
    return m_span.slice(start, stop);
  }

  auto index() {
    check_precondition("index()");
    return m_span.index();
  }

  ResidueRef operator[](size_t i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator const ResidueSpan& () const & {
    check_precondition("operator ResidueSpan&()");
    return m_span;
  }

  operator ResidueSpan& () & {
    check_precondition("operator ResidueSpan&()");
    return m_span;
  }

  operator ResidueSpan&& () && {
    check_precondition("operator ResidueSpan&()");
    return std::move(m_span);
  }

private:
  ResidueSpan m_span;
  bool m_is_split = false; /// indicates invalid span state
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_span.empty()) {
      throw DeadFrameAccessError(std::string("ResidueSmartSpan::") + func_name);
    }
    if (m_is_split) {
      throw SpanSplitError(std::string("ResidueSmartSpan::") + func_name);
    }
  }
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

} // namespace xmol::proxy::smart