#pragma once
#include "../proxy.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

/// Smart Atom reference proxy
class AtomSmartRef : public AtomRef, public FrameObserver<AtomSmartRef> {
public:
  AtomSmartRef(AtomRef&& atom);

private:
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

/// Smart Residue reference proxy
class ResidueSmartRef : public ResidueRef, public FrameObserver<ResidueSmartRef> {
public:
  ResidueSmartRef(ResidueRef&& res);

private:
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

/// Smart Molecule reference proxy
class MoleculeSmartRef : public MoleculeRef, public FrameObserver<MoleculeSmartRef> {
public:
  MoleculeSmartRef(MoleculeRef&& mol);

private:
  friend Frame;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};

} // namespace xmol::v1::proxy::smart