#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// ResidueSpan with parent frame changes tracking
class ResidueSmartSpan : public FrameObserver<ResidueSmartSpan> {
public:
  ResidueSmartSpan(ResidueRefSpan sel);

  /// Child atoms
  AtomRefSpan atoms() {
    check_precondition("atoms()");
    return m_span.atoms();
  };

  /// Parent molecules
  MoleculeRefSpan molecules(){
    check_precondition("molecules()");
    return m_span.molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> ResidueRefSpan filter(Predicate&& p) {
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

  ResidueRef operator[](int i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator ResidueRefSpan() {
    check_precondition("operator ResidueSpan&()");
    return m_span;
  }

private:
  ResidueRefSpan m_span;
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

} // namespace xmol::v1::proxy::smart