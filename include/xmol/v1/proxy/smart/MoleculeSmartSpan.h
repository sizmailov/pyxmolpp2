#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// MoleculeSpan with parent frame changes tracking
class MoleculeSmartSpan : public FrameObserver<MoleculeSmartSpan> {
public:
  MoleculeSmartSpan(MoleculeRefSpan sel);

  /// Coordinates
  CoordSpan coords() {
    check_precondition("coords()");
    return m_span.coords();
  };

  /// Children atoms
  AtomRefSpan atoms() {
    check_precondition("atoms()");
    return m_span.atoms();
  };

  /// Children residues
  ResidueRefSpan residues(){
    check_precondition("residues()");
    return m_span.residues();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> MoleculeRefSpan filter(Predicate&& p) {
    check_precondition("filter()");
    return m_span.filter(std::forward<Predicate>(p));
  }

  /// Check if element in selection
  [[nodiscard]] bool contains(const MoleculeRef& ref) const{
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

  MoleculeRef operator[](size_t i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator MoleculeRefSpan() {
    check_precondition("operator MoleculeSpan&()");
    return m_span;
  }

private:
  MoleculeRefSpan m_span;
  bool m_is_split = false; /// indicates invalid span state
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_span.empty()) {
      throw DeadFrameAccessError(std::string("MoleculeSmartSpan::") + func_name);
    }
    if (m_is_split) {
      throw SpanSplitError(std::string("MoleculeSmartSpan::") + func_name);
    }
  }
  friend Frame;
  struct MoleculeRefLessThanComparator;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};

} // namespace xmol::v1::proxy::smart