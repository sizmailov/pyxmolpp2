#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::proxy::smart {

/// ResidueSelection with parent frame changes tracking
class ResidueSmartSelection : public FrameObserver<ResidueSmartSelection> {
public:
  ResidueSmartSelection(ResidueSelection sel);

  /// Coordinates
  CoordSelection coords() {
    check_precondition("coords()");
    return m_selection.coords();
  };

  /// Child atoms
  AtomSelection atoms() {
    check_precondition("atoms()");
    return m_selection.atoms();
  };

  /// Parent molecules
  MoleculeSelection molecules(){
    check_precondition("molecules()");
    return m_selection.molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> ResidueSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_selection.filter(std::forward<Predicate>(p));
  }

  /// Inplace union
  void unite(const ResidueSelection& rhs){
    check_precondition("unite()");
    m_selection.unite(rhs);
  }

  /// Inplace difference
  void substract(const ResidueSelection& rhs) {
    check_precondition("substract()");
    if (empty()) {
      *this = ResidueSelection{};
    }
    m_selection.substract(rhs);
  }

  /// Inplace intersection
  void intersect(const ResidueSelection& rhs) {
    check_precondition("intersect()");
    if (empty()) {
      *this = ResidueSelection{};
    }
    m_selection.intersect(rhs);
  }

  /// Check if element in selection
  bool contains(const ResidueRef& ref) const{
    check_precondition("filter()");
    return m_selection.contains(ref);
  }

  /// Inplace union
  ResidueSmartSelection& operator|=(const ResidueSelection& rhs) {
    check_precondition("operator|=()");
    m_selection |=(rhs);
    return *this;
  };

  /// Inplace difference
  ResidueSmartSelection& operator-=(const ResidueSelection& rhs) {
    check_precondition("operator-=()");
    m_selection -= (rhs);
    if (empty()) {
      *this = ResidueSelection{};
    }
    return *this;
  };

  /// Inplace intersection
  ResidueSmartSelection& operator&=(const ResidueSelection& rhs) {
    check_precondition("operator&=()");
    m_selection &= rhs;
    if (empty()) {
      *this = ResidueSelection{};
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

  ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={}) {
    check_precondition("slice()");
    return m_selection.slice(start, stop, step);
  }

  auto index() {
    check_precondition("index()");
    return m_selection.index();
  }


  ResidueRef& operator[](size_t i) {
    check_precondition("operator[]()");
    return m_selection[i];
  }

  operator const ResidueSelection&() const & {
    check_precondition("operator const ResidueSelection&()");
    return m_selection;
  }

  operator ResidueSelection&() & {
    check_precondition("operator ResidueSelection&()");
    return m_selection;
  }

  operator ResidueSelection&&() &&{
    check_precondition("operator ResidueSelection&()");
    return std::move(m_selection);
  }

private:
  ResidueSelection m_selection;
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_selection.empty()) {
      throw DeadFrameAccessError(std::string("ResidueSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct ResidueRefLessThanComparator;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

} // namespace xmol::proxy::smart