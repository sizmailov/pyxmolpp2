#pragma once
#include "infos.h"
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

  MoleculeRef add_molecule(const ChainName& name = ChainName());
  [[nodiscard]] size_t n_atoms() const { return m_atom_info.size(); }
  [[nodiscard]] size_t n_residues() const { return m_residue_info.size(); }
  [[nodiscard]] size_t n_molecules() const { return m_molecule_info.size(); }

  [[nodiscard]] size_t n_atom_references() const { return selection::Observable<AtomRef>::observers.size(); }
  [[nodiscard]] size_t n_residue_references() const { return selection::Observable<ResidueRef>::observers.size(); }
  [[nodiscard]] size_t n_molecule_references() const { return selection::Observable<MoleculeRef>::observers.size(); }
  void reserve_molecules(size_t n) { m_molecule_info.reserve(n); };
  void reserve_atoms(size_t n) {
    m_atom_info.reserve(n);
    m_coordinates.reserve(n);
  };
  void reserve_residues(size_t n) { m_residue_info.reserve(n); };

private:
  ResidueRef add_residue(MoleculeRef& mol, const ResidueName& residueName, const ResidueId& residueId);
  AtomRef add_atom(ResidueRef& residue, const AtomName& atomName, const AtomId& atomId);

  void check_references_integrity();

  friend AtomRef;
  friend ResidueRef;
  friend MoleculeRef;

  std::vector<detail::AtomInfo> m_atom_info;
  std::vector<detail::ResidueInfo> m_residue_info;
  std::vector<detail::MoleculeInfo> m_molecule_info;
  std::vector<XYZ> m_coordinates;
};

} // namespace v1

} // namespace xmol