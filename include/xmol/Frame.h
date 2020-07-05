#pragma once
#include "base.h"
#include "proxy/smart/references.h" // <- can be moved to .cpp
#include "xmol/geom/UnitCell.h"
#include "xmol/utils/Observable.h"
#include <vector>

namespace xmol {

using FrameIndex = int32_t;

/// Molecular frame, owns all molecular data
class Frame : public utils::Observable<proxy::smart::AtomSmartRef>,
              public utils::Observable<proxy::smart::ResidueSmartRef>,
              public utils::Observable<proxy::smart::MoleculeSmartRef>,
              public utils::Observable<proxy::smart::AtomSmartSelection>,
              public utils::Observable<proxy::smart::ResidueSmartSelection>,
              public utils::Observable<proxy::smart::MoleculeSmartSelection>,
              public utils::Observable<proxy::smart::AtomSmartSpan>,
              public utils::Observable<proxy::smart::ResidueSmartSpan>,
              public utils::Observable<proxy::smart::MoleculeSmartSpan>,
              public utils::Observable<proxy::smart::CoordSmartSpan>,
              public utils::Observable<proxy::smart::CoordSmartSelection> {
public:
  /// Default constructor
  Frame() = default;

  /// Copy constructor
  Frame(const Frame& other);

  /// Move constructor
  Frame(Frame&& other);

  /// Copy assignment
  Frame& operator=(const Frame& other);

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
  [[nodiscard]] proxy::AtomSpan atoms();

  /// Residues of the frame
  [[nodiscard]] proxy::ResidueSpan residues();

  /// Molecules of the frame
  [[nodiscard]] proxy::MoleculeSpan molecules();

  /// Coordinates of the frame
  [[nodiscard]] proxy::CoordSpan coords();

  /// Current number of smart atom references
  template <typename Smart>[[nodiscard]] size_t n_references() const {
    static_assert(std::is_base_of_v<utils::Observable<Smart>, Frame>);
    return utils::Observable<Smart>::observers.size();
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
  proxy::MoleculeRef add_molecule();

  bool operator==(const Frame& rhs) const { return this == &rhs; }
  bool operator!=(const Frame& rhs) const { return this != &rhs; }

  std::optional<proxy::MoleculeRef> operator[](const MoleculeName& name);
  std::optional<proxy::MoleculeRef> operator[](const char* name);

  geom::UnitCell cell = geom::UnitCell(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));
  FrameIndex index = 0; /// Sequential index in trajectory
  double time = 0;      /// Time point in trajectory, a.u.

private:
  BaseResidue& add_residue(BaseMolecule& mol);
  BaseAtom& add_atom(BaseResidue& residue);

  inline CoordIndex index_of(const XYZ& coord) const noexcept {
    assert(m_coordinates.data() <= &coord);
    assert(&coord < m_coordinates.data() + m_coordinates.size());
    return &coord - m_coordinates.data();
  }

  inline AtomIndex index_of(const BaseAtom& atom) const noexcept {
    assert(m_atoms.data() <= &atom);
    assert(&atom < m_atoms.data() + m_atoms.size() );
    return &atom - m_atoms.data();
  }

  inline ResidueIndex index_of(const BaseResidue& residue) const noexcept {
    assert(m_residues.data() <= &residue);
    assert(&residue < m_residues.data() + m_residues.size() );
    return &residue - m_residues.data();
  }

  inline MoleculeIndex index_of(const BaseMolecule& molecule) const noexcept {
    assert(m_molecules.data() <= &molecule);
    assert(&molecule < m_molecules.data() + m_molecules.size() );
    return &molecule - m_molecules.data();
  }

  template <typename Observer> void reg(Observer& o) { utils::Observable<Observer>::add_observer(o); }

  XYZ& crd(BaseAtom& atom);

  void check_references_integrity();

  friend proxy::AtomRef;
  friend proxy::ResidueRef;
  friend proxy::MoleculeRef;

  friend proxy::CoordSpan;
  friend proxy::AtomSpan;
  friend proxy::ResidueSpan;
  friend proxy::MoleculeSpan;

  friend proxy::CoordSelection;
  friend proxy::AtomSelection;
  friend proxy::ResidueSelection;
  friend proxy::MoleculeSelection;

  friend proxy::smart::AtomSmartRef;
  friend proxy::smart::ResidueSmartRef;
  friend proxy::smart::MoleculeSmartRef;

  friend proxy::smart::CoordSmartSelection;
  friend proxy::smart::AtomSmartSelection;
  friend proxy::smart::ResidueSmartSelection;
  friend proxy::smart::MoleculeSmartSelection;

  friend proxy::smart::CoordSmartSpan;
  friend proxy::smart::AtomSmartSpan;
  friend proxy::smart::ResidueSmartSpan;
  friend proxy::smart::MoleculeSmartSpan;

  std::vector<BaseAtom> m_atoms;
  std::vector<BaseResidue> m_residues{};
  std::vector<BaseMolecule> m_molecules{};
  std::vector<XYZ> m_coordinates;

  void notify_frame_moved(Frame& other);
  void notify_frame_delete() const;
  void notify_atoms_move(BaseAtom* old_begin, BaseAtom* old_end, BaseAtom* new_begin) const;
  void notify_residues_move(BaseResidue* old_begin, BaseResidue* old_end, BaseResidue* new_begin) const;
  void notify_molecules_move(BaseMolecule* old_begin, BaseMolecule* old_end, BaseMolecule* new_begin) const;
  void notify_coordinates_move(XYZ* old_begin, XYZ* old_end, XYZ* new_begin) const;
};

} // namespace xmol