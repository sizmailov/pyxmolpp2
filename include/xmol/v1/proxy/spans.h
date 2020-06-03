#pragma once
#include "ProxySpan.h"
#include "../base.h"

namespace xmol::v1::proxy {

class AtomRefSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;
  ResidueRefSpan residues();
  MoleculeRefSpan molecules();
};

class ResidueRefSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  MoleculeRefSpan molecules();
};

class MoleculeRefSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  ResidueRefSpan residues();
};

} // namespace xmol::v1::proxy