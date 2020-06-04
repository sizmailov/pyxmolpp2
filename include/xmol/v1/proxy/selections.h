#pragma once
#include "Selection.h"
#include "proxy.h"

namespace xmol::v1::proxy {

class AtomSelection;
class ResidueSelection;
class MoleculeSelection;

class AtomSelection : public Selection<AtomRef> {
public:
  using Selection::Selection;
  AtomSelection(Selection&& rhs) : Selection(std::move(rhs)) {}
  ResidueSelection residues();
  MoleculeSelection molecules();

  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    return AtomSelection(internal_filter(std::forward<Predicate>(p)));
  }

  AtomSelection operator|=(const AtomSelection& rhs) {
    unite(rhs);
    return *this;
  };
  AtomSelection operator-=(const AtomSelection& rhs) {
    substract(rhs);
    return *this;
  };
  AtomSelection operator&=(const AtomSelection& rhs) {
    intersect(rhs);
    return *this;
  };
};

class ResidueSelection : public Selection<ResidueRef> {
public:
  using Selection::Selection;
  ResidueSelection(Selection&& rhs) : Selection(std::move(rhs)) {}
  AtomSelection atoms();
  MoleculeSelection molecules();

  ResidueSelection operator|=(const ResidueSelection& rhs) {
    unite(rhs);
    return *this;
  };
  ResidueSelection operator-=(const ResidueSelection& rhs) {
    substract(rhs);
    return *this;
  };
  ResidueSelection operator&=(const ResidueSelection& rhs) {
    intersect(rhs);
    return *this;
  };

  template <typename Predicate> ResidueSelection filter(Predicate&& p) {
    return ResidueSelection(internal_filter(std::forward<Predicate>(p)));
  }
};

class MoleculeSelection : public Selection<MoleculeRef> {
public:
  using Selection::Selection;
  MoleculeSelection(Selection&& rhs) : Selection(std::move(rhs)) {}
  AtomSelection atoms();
  ResidueSelection residues();

  MoleculeSelection operator|=(const MoleculeSelection& rhs) {
    unite(rhs);
    return *this;
  };
  MoleculeSelection operator-=(const MoleculeSelection& rhs) {
    substract(rhs);
    return *this;
  };
  MoleculeSelection operator&=(const MoleculeSelection& rhs) {
    intersect(rhs);
    return *this;
  };

  template <typename Predicate> MoleculeSelection filter(Predicate&& p) {
    return MoleculeSelection(internal_filter(std::forward<Predicate>(p)));
  }
};

} // namespace xmol::v1::proxy