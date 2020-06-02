#pragma once
#include "base.h"
#include "references.h"
#include "xmol/selection/Observable.h"
#include <vector>

namespace xmol {

namespace v1 {

class Frame : public selection::Observable<AtomRef>,
              public selection::Observable<ResidueRef>,
              public selection::Observable<MoleculeRef> {
public:
  struct base_tag {};
  Frame() = default;
  Frame(const Frame& other) = default;
  Frame(Frame&& other);
  Frame& operator=(const Frame& other) = default;
  Frame& operator=(Frame&& other);
  ~Frame();

  MoleculeRef add_molecule(const MoleculeName& name = MoleculeName());

  [[nodiscard]] size_t n_atoms() const { return atoms.size(); }
  [[nodiscard]] size_t n_residues() const { return residues.size(); }
  [[nodiscard]] size_t n_molecules() const { return molecules.size(); }
  [[nodiscard]] size_t n_atom_references() const { return selection::Observable<AtomRef>::observers.size(); }

  [[nodiscard]] size_t n_residue_references() const { return selection::Observable<ResidueRef>::observers.size(); }
  [[nodiscard]] size_t n_molecule_references() const { return selection::Observable<MoleculeRef>::observers.size(); }
  void reserve_molecules(size_t n) { molecules.reserve(n); };
  void reserve_atoms(size_t n) {
    atoms.reserve(n);
    coordinates.reserve(n);
  };
  void reserve_residues(size_t n) { residues.reserve(n); };

private:

  BaseMolecule& add_molecule(const MoleculeName& name, base_tag);
  BaseResidue& add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId, base_tag);
  BaseAtom& add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId, base_tag);

  ResidueRef add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId);
  AtomRef add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId);

  void check_references_integrity();

  friend AtomRef;
  friend ResidueRef;
  friend MoleculeRef;

  std::vector<BaseAtom> atoms;
  std::vector<BaseResidue> residues{};
  std::vector<BaseMolecule> molecules{};
  std::vector<XYZ> coordinates;
};

} // namespace v1

} // namespace xmol