#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// AtomSelection with parent frame changes tracking
class AtomSmartSelection : public FrameObserver<AtomSmartSelection> {
public:
  AtomSmartSelection(AtomSelection sel);

  /// Coordinates
  CoordSelection coords() {
    check_precondition("coords()");
    return m_selection.coords();
  };

  /// Parent residues
  ResidueSelection residues() {
    check_precondition("residues()");
    return m_selection.residues();
  };

  /// Parent molecules
  MoleculeSelection molecules() {
    check_precondition("molecules()");
    return m_selection.molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_selection.filter(std::forward<Predicate>(p));
  }

  /// Inplace union
  void unite(const AtomSelection& rhs) {
    check_precondition("unite()");
    m_selection.unite(rhs);
  }

  /// Inplace difference
  void substract(const AtomSelection& rhs) {
    check_precondition("substract()");
    if (empty()) {
      *this = AtomSelection{};
    }
    m_selection.substract(rhs);
  }

  /// Inplace intersection
  void intersect(const AtomSelection& rhs) {
    check_precondition("intersect()");
    if (empty()) {
      *this = AtomSelection{};
    }
    m_selection.intersect(rhs);
  }

  /// Check if element in selection
  [[nodiscard]] bool contains(const AtomRef& ref) const {
    check_precondition("filter()");
    return m_selection.contains(ref);
  }

  /// Inplace union
  AtomSmartSelection& operator|=(const AtomSelection& rhs) {
    check_precondition("operator|=()");
    m_selection |= (rhs);
    return *this;
  };

  /// Inplace difference
  AtomSmartSelection& operator-=(const AtomSelection& rhs) {
    check_precondition("operator-=()");
    m_selection -= (rhs);
    if (empty()) {
      *this = AtomSelection{};
    }
    return *this;
  };

  /// Inplace intersection
  AtomSmartSelection& operator&=(const AtomSelection& rhs) {
    check_precondition("operator&=()");
    m_selection &= rhs;
    if (empty()) {
      *this = AtomSelection{};
    }
    return *this;
  };

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

  AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={}) {
    check_precondition("slice()");
    return m_selection.slice(start, stop, step);
  }

  auto index() {
    check_precondition("index()");
    return m_selection.index();
  }

  void guess_mass() {
    check_precondition("guess_mass()");
    m_selection.guess_mass();
  }

  auto inertia_tensor() -> Eigen::Matrix3d;
  auto alignment_to(AtomSpan& rhs, bool weighted=false)  -> geom::affine::Transformation3d;
  auto alignment_to(AtomSelection& rhs, bool weighted=false)  -> geom::affine::Transformation3d;

  double rmsd(AtomSpan& rhs, bool weighted=false) {
    check_precondition("rmsd()");
    return m_selection.rmsd(rhs, weighted);
  }

  double rmsd(AtomSelection& rhs, bool weighted=false) {
    check_precondition("rmsd()");
    return m_selection.rmsd(rhs, weighted);
  }

  XYZ mean(bool weighted=false) {
    check_precondition("mean()");
    return m_selection.mean(weighted);
  }

  AtomRef& operator[](size_t i) {
    check_precondition("operator[]()");
    return m_selection[i];
  }

  operator const AtomSelection&() const & {
    check_precondition("operator const AtomSelection&()");
    return m_selection;
  }

  operator AtomSelection& () &{
    check_precondition("operator AtomSelection&()");
    return m_selection;
  }

  operator AtomSelection&&() &&{
    check_precondition("operator AtomSelection&()");
    return std::move(m_selection);
  }

private:
  AtomSelection m_selection;
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_selection.empty()) {
      throw DeadFrameAccessError(std::string("AtomSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct AtomRefLessThanComparator;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

} // namespace xmol::proxy::smart