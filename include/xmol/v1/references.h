#pragma once
#include "FrameObserver.h"
#include "proxy.h"

namespace xmol {
namespace v1 {

class AtomRef : public proxy::Atom, public FrameObserver<AtomRef> {
public:
  AtomRef(Atom&& atom);
private:
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

class ResidueRef : public proxy::Residue, public FrameObserver<ResidueRef> {
public:
  ResidueRef(proxy::Residue&& res);
private:
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

class MoleculeRef : public proxy::Molecule, public FrameObserver<MoleculeRef> {
public:
  MoleculeRef(proxy::Molecule&& mol);
private:
  friend Frame;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};
} // namespace v1
} // namespace xmol