#pragma once
#include "../proxy.h"
#include "FrameObserver.h"

namespace xmol::v1 {

namespace proxy {

namespace smart {

class AtomSmartRef : public proxy::AtomRef, public FrameObserver<AtomSmartRef> {
public:
  AtomSmartRef(AtomRef&& atom);

private:
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

class ResidueSmartRef : public proxy::ResidueRef, public FrameObserver<ResidueSmartRef> {
public:
  ResidueSmartRef(proxy::ResidueRef&& res);

private:
  friend Frame;
  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);
};

class MoleculeSmartRef : public proxy::MoleculeRef, public FrameObserver<MoleculeSmartRef> {
public:
  MoleculeSmartRef(proxy::MoleculeRef&& mol);

private:
  friend Frame;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
};
} // namespace smart

inline namespace aliases {
using AtomSmartRef = smart::AtomSmartRef;
using ResidueSmartRef = smart::ResidueSmartRef;
using MoleculeSmartRef = smart::MoleculeSmartRef;
} // namespace aliases

} // namespace proxy

inline namespace aliases {
using namespace proxy::aliases;
} // namespace aliases

} // namespace xmol::v1