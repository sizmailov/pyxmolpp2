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
  Frame() = default;
  Frame(const Frame& other) = default;
  Frame(Frame&& other);
  Frame& operator=(const Frame& other) = default;
  Frame& operator=(Frame&& other);
  ~Frame();


  [[nodiscard]] size_t n_atoms() const { return m_atoms.size(); }
  [[nodiscard]] size_t n_residues() const { return m_residues.size(); }
  [[nodiscard]] size_t n_molecules() const { return m_molecules.size(); }

  [[nodiscard]] proxy::ProxySpan<proxy::Atom, BaseAtom> atoms();
  [[nodiscard]] proxy::ProxySpan<proxy::Residue, BaseResidue> residues();
  [[nodiscard]] proxy::ProxySpan<proxy::Molecule, BaseMolecule> molecules();
  [[nodiscard]] future::Span<XYZ> coordinates();

  [[nodiscard]] size_t n_atom_references() const { return selection::Observable<AtomRef>::observers.size(); }
  [[nodiscard]] size_t n_residue_references() const { return selection::Observable<ResidueRef>::observers.size(); }
  [[nodiscard]] size_t n_molecule_references() const { return selection::Observable<MoleculeRef>::observers.size(); }

  void reserve_molecules(size_t n);
  void reserve_atoms(size_t n);
  void reserve_residues(size_t n);

  proxy::Molecule add_molecule(const MoleculeName& name);
private:
  BaseResidue& add_residue(BaseMolecule& mol, const ResidueName& residueName, const ResidueId& residueId);
  BaseAtom& add_atom(BaseResidue& residue, const AtomName& atomName, const AtomId& atomId);

  template<typename Observer>
  void reg(Observer& o){
    selection::Observable<Observer>::add_observer(o);
  }

  XYZ& crd(BaseAtom& atom);

  void check_references_integrity();

  friend AtomRef;
  friend ResidueRef;
  friend MoleculeRef;

  friend proxy::Atom;
  friend proxy::Residue;
  friend proxy::Molecule;

  std::vector<BaseAtom> m_atoms;
  std::vector<BaseResidue> m_residues{};
  std::vector<BaseMolecule> m_molecules{};
  std::vector<XYZ> m_coordinates;
};

} // namespace v1

} // namespace xmol