#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

class AtomSmartSelection : public AtomSelection, public FrameObserver<AtomSmartSelection> {
public:
  AtomSmartSelection(AtomSelection sel);

  /// Parent residues
  ResidueSelection residues() {
    check_precondition("residues()");
    return AtomSelection::residues();
  };

  /// Parent molecules
  MoleculeSelection molecules(){
    check_precondition("molecules()");
    return AtomSelection::molecules();
  };

  /// Returns selection with atoms that match predicate
  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    check_precondition("filter()");
    return AtomSelection::filter(std::forward<Predicate>(p));
  }

  /// Inplace union
  void unite(const Selection& rhs){
    check_precondition("unite()");
    AtomSelection::unite(rhs);

  }

  /// Inplace difference
  void substract(const Selection& rhs){
    check_precondition("substract()");
    AtomSelection::substract(rhs);

  }

  /// Inplace intersection
  void intersect(const Selection& rhs){
    check_precondition("intersect()");
    AtomSelection::intersect(rhs);
  }

  /// Check if element in selection
  bool contains(const AtomRef& ref) const{
    check_precondition("filter()");
    return AtomSelection::contains(ref);
  }

  /// Inplace union
  AtomSmartSelection& operator|=(const AtomSelection& rhs) {
    check_precondition("operator|=()");
    AtomSelection::operator|=(rhs);
    return *this;
  };

  /// Inplace difference
  AtomSmartSelection& operator-=(const AtomSelection& rhs) {
    check_precondition("operator-=()");
    AtomSelection::operator-=(rhs);
    return *this;
  };

  /// Inplace intersection
  AtomSmartSelection& operator&=(const AtomSelection& rhs) {
    check_precondition("operator&=()");
    AtomSelection::operator&=(rhs);
    return *this;
  };

  [[nodiscard]] auto begin() {
    check_precondition("begin()");
    return AtomSelection::begin();
  }
  [[nodiscard]] auto end() {
    check_precondition("end()");
    return AtomSelection::end();
  }
  [[nodiscard]] size_t size() const {
    check_precondition("size()");
    return AtomSelection::size();
  }
  [[nodiscard]] size_t empty() const {
    check_precondition("empty()");
    return AtomSelection::empty();
  }

  AtomRef& operator[](int i) {
    check_precondition("operator[]()");
    return AtomSelection::operator[](i);
  }

private:
  inline void check_precondition(const char* func_name) const {
    if (!is_bound_to_frame() && !m_data.empty()) {
      throw DeadFrameAccessError(std::string("AtomSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct AtomRefLessThanComparator;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

} // namespace xmol::v1::proxy::smart