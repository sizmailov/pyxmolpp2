#pragma once
/** @file
 * @brief Ligthweight selections of Atom, Reference and Molecule references.
 */

#include "Selection.h"
#include "proxy.h"

namespace xmol::v1::proxy {

/// @breif Ordered set of @ref AtomRef from single @ref Frame
///
/// Mixing references from different frames within single AtomSelection is prohibited
class AtomSelection : public Selection<AtomRef> {
public:
  using Selection::Selection;
  /// Construct from parent
  AtomSelection(Selection&& rhs) : Selection(std::move(rhs)) {}

  /// Parent residues
  ResidueSelection residues();
  /// Parent molecules
  MoleculeSelection molecules();

  /// Returns selection with atoms that match predicate
  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    return AtomSelection(internal_filter(std::forward<Predicate>(p)));
  }

  /// Inplace union
  AtomSelection& operator|=(const AtomSelection& rhs) {
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  AtomSelection& operator-=(const AtomSelection& rhs) {
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  AtomSelection& operator&=(const AtomSelection& rhs) {
    intersect(rhs);
    return *this;
  };

  /// Create smart selection from this
  smart::AtomSmartSelection smart();

private:
  friend smart::AtomSmartSelection;
  Frame* frame_ptr(){
    return empty()? nullptr : &m_data[0].frame();
  }
};

/// @breif Ordered set of @ref ResidueRef from single @ref Frame
///
/// Mixing references from different frames within single ResidueSelection is prohibited
class ResidueSelection : public Selection<ResidueRef> {
public:
  using Selection::Selection;
  ResidueSelection(Selection&& rhs) : Selection(std::move(rhs)) {}

  /// Children atoms of the residues
  AtomSelection atoms();

  /// Parent molecules
  MoleculeSelection molecules();

  /// Inplace union
  ResidueSelection& operator|=(const ResidueSelection& rhs) {
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  ResidueSelection& operator-=(const ResidueSelection& rhs) {
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  ResidueSelection& operator&=(const ResidueSelection& rhs) {
    intersect(rhs);
    return *this;
  };

  /// Returns selection with residues that match predicate
  template <typename Predicate> ResidueSelection filter(Predicate&& p) {
    return ResidueSelection(internal_filter(std::forward<Predicate>(p)));
  }

  /// Create smart selection from this
  smart::ResidueSmartSelection smart();

private:
  friend smart::ResidueSmartSelection;
  Frame* frame_ptr(){
    return empty()? nullptr : &m_data[0].frame();
  }
};

/// @breif Ordered set of @ref MoleculeRef from single @ref Frame
///
/// Mixing references from different frames within single MoleculeSelection is prohibited
class MoleculeSelection : public Selection<MoleculeRef> {
public:
  using Selection::Selection;
  MoleculeSelection(Selection&& rhs) : Selection(std::move(rhs)) {}

  /// Children atoms of the molecules
  AtomSelection atoms();

  /// Children residues of the molecules
  ResidueSelection residues();

  /// Inplace union
  MoleculeSelection operator|=(const MoleculeSelection& rhs) {
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  MoleculeSelection operator-=(const MoleculeSelection& rhs) {
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  MoleculeSelection operator&=(const MoleculeSelection& rhs) {
    intersect(rhs);
    return *this;
  };

  /// Returns selection with molecules that match predicate
  template <typename Predicate> MoleculeSelection filter(Predicate&& p) {
    return MoleculeSelection(internal_filter(std::forward<Predicate>(p)));
  }

  /// Create smart selection from this
  smart::MoleculeSmartSelection smart();

private:
  friend smart::MoleculeSmartSelection;
  Frame* frame_ptr(){
    return empty()? nullptr : &m_data[0].frame();
  }
};

} // namespace xmol::v1::proxy