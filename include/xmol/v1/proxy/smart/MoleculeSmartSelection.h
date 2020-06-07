#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// MoleculeSelection with parent frame changes tracking
class MoleculeSmartSelection : public FrameObserver<MoleculeSmartSelection> {
public:
  MoleculeSmartSelection(MoleculeSelection sel);

  /// Children atoms
  AtomSelection atoms() {
    check_precondition("atoms()");
    return m_selection.atoms();
  };

  /// Children residues
  ResidueSelection residues(){
    check_precondition("residues()");
    return m_selection.residues();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> MoleculeSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return m_selection.filter(std::forward<Predicate>(p));
  }

  /// Inplace union
  void unite(const MoleculeSelection& rhs){
    check_precondition("unite()");
    m_selection.unite(rhs);

  }

  /// Inplace difference
  void substract(const MoleculeSelection& rhs){
    check_precondition("substract()");
    m_selection.substract(rhs);
  }

  /// Inplace intersection
  void intersect(const MoleculeSelection& rhs){
    check_precondition("intersect()");
    m_selection.intersect(rhs);
  }

  /// Check if element in selection
  [[nodiscard]] bool contains(const MoleculeRef& ref) const{
    check_precondition("filter()");
    return m_selection.contains(ref);
  }

  /// Inplace union
  MoleculeSmartSelection& operator|=(const MoleculeSelection& rhs) {
    check_precondition("operator|=()");
    m_selection |=(rhs);
    return *this;
  };

  /// Inplace difference
  MoleculeSmartSelection& operator-=(const MoleculeSelection& rhs) {
    check_precondition("operator-=()");
    m_selection -=(rhs);
    return *this;
  };

  /// Inplace intersection
  MoleculeSmartSelection& operator&=(const MoleculeSelection& rhs) {
    check_precondition("operator&=()");
    m_selection &= rhs;
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

  MoleculeRef& operator[](size_t i) {
    check_precondition("operator[]()");
    return m_selection[i];
  }

  operator const MoleculeSelection&() const {
    check_precondition("operator const MoleculeSelection&()");
    return m_selection;
  }

  operator MoleculeSelection&() {
    check_precondition("operator MoleculeSelection&()");
    return m_selection;
  }

private:
  MoleculeSelection m_selection;
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_selection.empty()) {
      throw DeadFrameAccessError(std::string("MoleculeSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct MoleculeRefLessThanComparator;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};

} // namespace xmol::v1::proxy::smart