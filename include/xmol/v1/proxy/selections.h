#pragma once
#include "Selection.h"
#include "proxy.h"

namespace xmol::v1::proxy {

class AtomSelection;
class ResidueSelection;
class MoleculeSelection;

class AtomSelection : public Selection<AtomRef> {
public:
  using Selection::Selection;
  ResidueSelection residues();
  MoleculeSelection molecules();
};

class ResidueSelection : public Selection<ResidueRef> {
public:
  using Selection::Selection;
  AtomSelection atoms();
  MoleculeSelection molecules();
};

class MoleculeSelection : public Selection<MoleculeRef> {
public:
  using Selection::Selection;
  AtomSelection atoms();
  ResidueSelection residues();
};

} // namespace xmol::v1::proxy