#pragma once
#include "future/span.h"
#include "fwd.h"

namespace xmol {
namespace v1 {

struct BaseAtom;
struct BaseResidue;
struct BaseMolecule;

struct BaseAtom {
  AtomName name;
  AtomId id;
  BaseResidue* residue = nullptr;
};

struct BaseResidue {
  ResidueName name;
  ResidueId id;
  future::Span<BaseAtom> atoms;
  BaseMolecule* molecule = nullptr;
};

struct BaseMolecule {
  Frame* frame;
  MoleculeName name;
  future::Span<BaseResidue> residues;
};

} // namespace v1
} // namespace xmol