#pragma once
#include "base.h"
#include "proxy/smart/references.h"
#include "xmol/selection/Observable.h"
#include <vector>

namespace xmol::v1 {

/// Molecular frame, owns all molecular data
class Frame : public selection::Observable<AtomSmartRef>,
              public selection::Observable<ResidueSmartRef>,
              public selection::Observable<MoleculeSmartRef> {
public:
  /// Default constructor
  Frame() = default;

  /// Copy constructor
  Frame(const Frame& other) = default;

  /// Move constructor
  Frame(Frame&& other);

  /// Copy assignment
  Frame& operator=(const Frame& other) = default;

  /// Move assignment
  Frame& operator=(Frame&& other);

  /// Destructor
  ~Frame();

  /// Total number of atoms in the frame
  [[nodiscard]] size_t n_atoms() const { return m_atoms.size(); }

  /// Total number of residues in the frame
  [[nodiscard]] size_t n_residues() const { return m_residues.size(); }

  /// Total number of molecules in the frame
  [[nodiscard]] size_t n_molecules() const { return m_molecules.size(); }

  /// Atoms of the frame
  [[nodiscard]] AtomRefSpan atoms();

  /// Residues of the frame
  [[nodiscard]] ResidueRefSpan residues();

  /// Molecules of the frame
  [[nodiscard]] MoleculeRefSpan molecules();

  /// Coordinates of the frame
  [[nodiscard]] future::Span<XYZ> coordinates();

  /// Current number of smart atom references
  [[nodiscard]] size_t n_atom_references() const {
    return selection::Observable<AtomSmartRef>::observers.size();
  }

  /// Current number of smart residue references
  [[nodiscard]] size_t n_residue_references() const {
    return selection::Observable<ResidueSmartRef>::observers.size();
  }

  /// Current number of smart molecule references
  [[nodiscard]] size_t n_molecule_references() const {
    return selection::Observable<MoleculeSmartRef>::observers.size();
  }

  /// @brief Preallocate space for n atoms
  ///
  /// Invalidates all kinds of non-smart atom references including AtomRef, AtomRefSpan and
  /// AtomSelection
  void reserve_atoms(size_t n);

  /// @brief Preallocate space for n residues
  ///
  /// Invalidates all kinds of non-smart residue references ResidueRef, ResidueRefSpan and
  /// ResidueSelection
  void reserve_residues(size_t n);

  /// @brief Preallocate space for n molecules
  ///
  /// Invalidates all kinds of non-smart molecule references including MoleculeRef, MoleculeRefSpan and
  /// MoleculeSelection
  void reserve_molecules(size_t n);

  /// @brief Add new molecules at the end of frame and return reference to it
  ///
  /// Invalidates all kinds of non-smart molecule references including MoleculeRef, MoleculeRefSpan and
  /// MoleculeSelection
  ///
  /// Appropriate reserve_molecules() call prevents references invalidation
  MoleculeRef add_molecule(const MoleculeName& name);

private:
  BaseResidue& add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId);
  BaseAtom& add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId);

  template <typename Observer> void reg(Observer& o) { selection::Observable<Observer>::add_observer(o); }

  XYZ& crd(BaseAtom& atom);

  void check_references_integrity();

  friend AtomSmartRef;
  friend ResidueSmartRef;
  friend MoleculeSmartRef;

  friend AtomRef;
  friend ResidueRef;
  friend MoleculeRef;

  std::vector<BaseAtom> m_atoms;
  std::vector<BaseResidue> m_residues{};
  std::vector<BaseMolecule> m_molecules{};
  std::vector<XYZ> m_coordinates;
};

} // namespace xmol::v1