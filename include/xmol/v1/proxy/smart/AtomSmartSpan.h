#pragma once
#include "../spans-impl.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// AtomSpan with parent frame changes tracking
class AtomSmartSpan : public FrameObserver<AtomSmartSpan> {
public:
  AtomSmartSpan(AtomRefSpan sel);

  /// Parent residues
  ResidueRefSpan residues() {
    check_precondition("residues()");
    return m_span.residues();
  };

  /// Parent molecules
  MoleculeRefSpan molecules() {
    check_precondition("molecules()");
    return m_span.molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_span.filter(std::forward<Predicate>(p));
  }

  /// Check if element in selection
  [[nodiscard]] bool contains(const AtomRef& ref) const {
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

  AtomRef operator[](int i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator AtomRefSpan() {
    check_precondition("operator AtomRefSpan()");
    return m_span;
  }

private:
  AtomRefSpan m_span;
  bool m_is_split = false; /// indicates invalid span state
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_span.empty()) {
      throw DeadFrameAccessError(std::string("AtomSmartSpan::") + func_name);
    }
    if (m_is_split) {
      throw SpanSplitError(std::string("AtomSmartSpan::") + func_name);
    }
  }
  friend Frame;
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

} // namespace xmol::v1::proxy::smart