#pragma once
#include "ProxySpan.h"
#include "../base.h"

namespace xmol::v1::proxy {

class AtomRefSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;
  ResidueRefSpan residues();
  MoleculeRefSpan molecules();

  template<typename Predicate>
  AtomSelection filter(Predicate&& p);
};

class ResidueRefSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  MoleculeRefSpan molecules();

  template<typename Predicate>
  ResidueSelection filter(Predicate&& p);
};

class MoleculeRefSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  ResidueRefSpan residues();

  template<typename Predicate>
  MoleculeSelection filter(Predicate&& p);

};

} // namespace xmol::v1::proxy