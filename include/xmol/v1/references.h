#pragma once
#include "FrameObserver.h"
#include "proxy.h"

namespace xmol {
namespace v1 {

class AtomRef : public proxy::Atom, public FrameObserver<AtomRef> {
private:
  friend Frame;
  explicit AtomRef(Atom&& atom);
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

class ResidueRef : public proxy::Residue, public FrameObserver<ResidueRef> {
public:
  AtomRef add_atom(const AtomName& atomName, const AtomId& atomId);
private:
  friend Frame;
  explicit ResidueRef(proxy::Residue&& res);
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

class MoleculeRef : public proxy::Molecule, public FrameObserver<MoleculeRef> {
public:
  ResidueRef add_residue(const ResidueName& residueName, const ResidueId& residueId);
private:
  friend Frame;
  explicit MoleculeRef(proxy::Molecule&& mol);
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};
} // namespace v1
} // namespace xmol