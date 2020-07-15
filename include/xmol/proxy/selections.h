#pragma once
/** @file
 * @brief Ligthweight selections of Atom, Reference and Molecule references.
 */

#include "Selection.h"
#include "proxy.h"

namespace xmol::proxy {

class MultipleFramesSelectionError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class CoordSelectionSizeMismatchError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};


/// Ordered set of atomic coordinates
///
/// Unlike other selections this one does NOT support set operations
/// Once created, the selection cannot be altered
class CoordSelection : public Selection<CoordRef> {
public:
  CoordSelection() = default;
  CoordSelection(CoordSpan rhs) : Selection(rhs.begin(), rhs.end()), m_frame(empty() ? nullptr : rhs.m_frame) {}

  template <typename Container>
  CoordSelection(Frame& frame, Container&& container, bool sorted_and_unique = false)
      : Selection(std::forward<Container>(container), sorted_and_unique), m_frame(empty() ? nullptr : &frame) {}

  template <typename Predicate> CoordSelection filter(Predicate&& p) {
    if (empty()) {
      return {};
    }
    return CoordSelection(*m_frame, internal_filter(std::forward<Predicate>(p)));
  }
  std::vector<CoordIndex> index() const;

  geom::affine::Transformation3d alignment_to(CoordSpan& other);
  geom::affine::Transformation3d alignment_to(CoordSelection& other);

  double rmsd(CoordSpan& other);
  double rmsd(CoordSelection& other);


  Eigen::Matrix3d inertia_tensor();


  /// Copy of seleciton coordinates
  CoordEigenMatrix _eigen();

  /// Assign selection coordinates
  void _eigen(const CoordEigenMatrix& matrix);

  void apply(const geom::affine::Transformation3d& t);
  void apply(const geom::affine::UniformScale3d& t);
  void apply(const geom::affine::Rotation3d& t);
  void apply(const geom::affine::Translation3d& t);
  XYZ mean();

  CoordSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={});

  smart::CoordSmartSelection smart();

protected:
  Frame* m_frame = nullptr;

private:
  friend CoordSpan;
  friend AtomSelection;
  friend ResidueSelection;
  friend MoleculeSelection;
  friend smart::CoordSmartSelection;
};

/// @breif Ordered set of @ref AtomRef from single @ref Frame
///
/// Mixing references from different frames within single AtomSelection is prohibited
class AtomSelection : public Selection<AtomRef> {
public:
  using Selection::Selection;
  /// Construct from parent
  AtomSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  AtomSelection(AtomSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

  /// Coordinates
  CoordSelection coords();
  /// Parent residues
  ResidueSelection residues();
  /// Parent molecules
  MoleculeSelection molecules();

  /// Returns selection with atoms that match predicate
  template <typename Predicate> AtomSelection filter(Predicate&& p) {
    return AtomSelection(internal_filter(std::forward<Predicate>(p)));
  }

  std::vector<AtomIndex> index() const;

  AtomSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={});

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

  /// Guess atom mass by atom name
  void guess_mass();

  /// Guess atom mass by atom name
  [[nodiscard]] Eigen::Matrix3d inertia_tensor();

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSpan& rhs, bool weighted=false);

  /// Calc alignment to another set of atoms
  [[nodiscard]] geom::affine::Transformation3d alignment_to(AtomSelection& rhs, bool weighted=false);

  [[nodiscard]] double rmsd(AtomSelection& rhs, bool weighted=false);
  [[nodiscard]] double rmsd(AtomSpan& rhs, bool weighted=false);

  [[nodiscard]] XYZ mean(bool weighted=false);

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
  const Frame* frame_ptr() const { return empty() ? nullptr : &m_data[0].frame(); }
};

/// @breif Ordered set of @ref ResidueRef from single @ref Frame
///
/// Mixing references from different frames within single ResidueSelection is prohibited
class ResidueSelection : public Selection<ResidueRef> {
public:
  using Selection::Selection;
  ResidueSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  ResidueSelection(ResidueSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

  /// Coordinates
  CoordSelection coords();

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

  std::vector<ResidueIndex> index() const;


  ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={});

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
  const Frame* frame_ptr() const { return empty() ? nullptr : &m_data[0].frame(); }
};

/// @breif Ordered set of @ref MoleculeRef from single @ref Frame
///
/// Mixing references from different frames within single MoleculeSelection is prohibited
class MoleculeSelection : public Selection<MoleculeRef> {
public:
  using Selection::Selection;
  MoleculeSelection(Selection&& rhs) : Selection(std::move(rhs)) { check_invariants("ctor"); }
  MoleculeSelection(MoleculeSpan rhs) : Selection(rhs.begin(), rhs.end()) {}

  /// Coordinates
  CoordSelection coords();

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

  std::vector<MoleculeIndex> index() const;

  MoleculeSelection slice(std::optional<size_t> start, std::optional<size_t> stop={}, std::optional<size_t> step={});

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
  const Frame* frame_ptr() const { return empty() ? nullptr : &m_data[0].frame(); }
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

} // namespace xmol::proxy