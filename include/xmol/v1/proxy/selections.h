#pragma once
/** @file
 * @brief Ligthweight selections of Atom, Reference and Molecule references.
 */

#include "Selection.h"
#include "proxy.h"

namespace xmol::v1::proxy {

class MultipleFramesSelectionError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/// @breif Ordered set of @ref AtomRef from single @ref Frame
///
/// Mixing references from different frames within single AtomSelection is prohibited
class AtomSelection : public Selection<AtomRef> {
public:
  using Selection::Selection;
  /// Construct from parent
  AtomSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  AtomSelection(AtomRefSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

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
    check_frame("operator|=()", rhs);
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  AtomSelection& operator-=(const AtomSelection& rhs) {
    check_frame("operator-=()", rhs);
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  AtomSelection& operator&=(const AtomSelection& rhs) {
    check_frame("operator&=()", rhs);
    intersect(rhs);
    return *this;
  };

  /// Create smart selection from this
  smart::AtomSmartSelection smart();

private:
  inline void check_invariants(const char* func_name) {
    if (!empty() && &m_data.front().frame() != &m_data.back().frame()) {
      throw MultipleFramesSelectionError(std::string("AtomSelection::") + func_name);
    }
  }
  inline void check_frame(const char* func_name, const AtomSelection& other) {
    if (!empty() && !other.empty() && &m_data.front().frame() != &other.m_data.front().frame()) {
      throw MultipleFramesSelectionError(std::string("AtomSelection::") + func_name);
    }
  }
  friend smart::AtomSmartSelection;
  Frame* frame_ptr() { return empty() ? nullptr : &m_data[0].frame(); }
};

/// @breif Ordered set of @ref ResidueRef from single @ref Frame
///
/// Mixing references from different frames within single ResidueSelection is prohibited
class ResidueSelection : public Selection<ResidueRef> {
public:
  using Selection::Selection;
  ResidueSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  ResidueSelection(ResidueRefSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

  /// Children atoms of the residues
  AtomSelection atoms();

  /// Parent molecules
  MoleculeSelection molecules();

  /// Inplace union
  ResidueSelection& operator|=(const ResidueSelection& rhs) {
    check_frame("operator|=()", rhs);
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  ResidueSelection& operator-=(const ResidueSelection& rhs) {
    check_frame("operator-=()", rhs);
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  ResidueSelection& operator&=(const ResidueSelection& rhs) {
    check_frame("operator&=()", rhs);
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
  inline void check_invariants(const char* func_name) {
    if (!empty() && &m_data.front().frame() != &m_data.back().frame()) {
      throw MultipleFramesSelectionError(std::string("ResidueSelection::") + func_name);
    }
  }
  inline void check_frame(const char* func_name, const ResidueSelection& other) {
    if (!empty() && !other.empty() && &m_data.front().frame() != &other.m_data.front().frame()) {
      throw MultipleFramesSelectionError(std::string("ResidueSelection::") + func_name);
    }
  }
  friend smart::ResidueSmartSelection;
  Frame* frame_ptr() { return empty() ? nullptr : &m_data[0].frame(); }
};

/// @breif Ordered set of @ref MoleculeRef from single @ref Frame
///
/// Mixing references from different frames within single MoleculeSelection is prohibited
class MoleculeSelection : public Selection<MoleculeRef> {
public:
  using Selection::Selection;
  MoleculeSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  MoleculeSelection(MoleculeRefSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

  /// Children atoms of the molecules
  AtomSelection atoms();

  /// Children residues of the molecules
  ResidueSelection residues();

  /// Inplace union
  MoleculeSelection operator|=(const MoleculeSelection& rhs) {
    check_frame("operator|=()", rhs);
    unite(rhs);
    return *this;
  };

  /// Inplace difference
  MoleculeSelection operator-=(const MoleculeSelection& rhs) {
    check_frame("operator-=()", rhs);
    substract(rhs);
    return *this;
  };

  /// Inplace intersection
  MoleculeSelection operator&=(const MoleculeSelection& rhs) {
    check_frame("operator&=()", rhs);
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
  inline void check_invariants(const char* func_name) {
    if (!empty() && &m_data.front().frame() != &m_data.back().frame()) {
      throw MultipleFramesSelectionError(std::string("MoleculeSelection::") + func_name);
    }
  }
  inline void check_frame(const char* func_name, const MoleculeSelection& other) {
    if (!empty() && !other.empty() && &m_data.front().frame() != &other.m_data.front().frame()) {
      throw MultipleFramesSelectionError(std::string("MoleculeSelection::") + func_name);
    }
  }
  friend smart::MoleculeSmartSelection;
  Frame* frame_ptr() { return empty() ? nullptr : &m_data[0].frame(); }
};

AtomSelection operator|(const AtomSelection& lhs, const AtomSelection& rhs);
AtomSelection operator-(const AtomSelection& lhs, const AtomSelection& rhs);
AtomSelection operator&(const AtomSelection& lhs, const AtomSelection& rhs);

ResidueSelection operator|(const ResidueSelection& lhs, const ResidueSelection& rhs);
ResidueSelection operator-(const ResidueSelection& lhs, const ResidueSelection& rhs);
ResidueSelection operator&(const ResidueSelection& lhs, const ResidueSelection& rhs);

MoleculeSelection operator|(const MoleculeSelection& lhs, const MoleculeSelection& rhs);
MoleculeSelection operator-(const MoleculeSelection& lhs, const MoleculeSelection& rhs);
MoleculeSelection operator&(const MoleculeSelection& lhs, const MoleculeSelection& rhs);

} // namespace xmol::v1::proxy