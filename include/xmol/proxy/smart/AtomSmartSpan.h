#pragma once
#include "../spans-impl.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// AtomSpan with parent frame changes tracking
class AtomSmartSpan : public FrameObserver<AtomSmartSpan> {
public:
  AtomSmartSpan(AtomSpan sel);

  /// Coordinates
  CoordSpan coords() {
    check_precondition("coords()");
    return m_span.coords();
  };

  /// Parent residues
  ResidueSpan residues() {
    check_precondition("residues()");
    return m_span.residues();
  };

  /// Parent molecules
  MoleculeSpan molecules() {
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

  AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) {
    check_precondition("slice()");
    return m_span.slice(start, stop, step);
  }

  AtomSpan slice(std::optional<size_t> start, std::optional<size_t> stop) {
    check_precondition("slice()");
    return m_span.slice(start, stop);
  }

  auto index() {
    check_precondition("index()");
    return m_span.index();
  }

  void guess_mass() {
    check_precondition("guess_mass()");
    m_span.guess_mass();
  }

  auto inertia_tensor() -> Eigen::Matrix3d;
  auto alignment_to(AtomSpan& rhs, bool weighted=false) -> geom::affine::Transformation3d;
  auto alignment_to(AtomSelection& rhs, bool weighted=false) -> geom::affine::Transformation3d;

  double rmsd(AtomSpan& rhs, bool weighted=false) {
    check_precondition("rmsd()");
    return m_span.rmsd(rhs, weighted);
  }

  double rmsd(AtomSelection& rhs, bool weighted=false) {
    check_precondition("rmsd()");
    return m_span.rmsd(rhs, weighted);
  }

  XYZ mean(bool weighted=false) {
    check_precondition("mean()");
    return m_span.mean(weighted);
  }

  AtomRef operator[](size_t i) {
    check_precondition("operator[]()");
    return m_span[i];
  }

  operator const AtomSpan& () const & {
    check_precondition("operator AtomSpan()");
    return m_span;
  }

  operator AtomSpan& () & {
    check_precondition("operator AtomSpan()");
    return m_span;
  }

  operator AtomSpan&& () && {
    check_precondition("operator AtomSpan()");
    return std::move(m_span);
  }

private:
  AtomSpan m_span;
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

} // namespace xmol::proxy::smart