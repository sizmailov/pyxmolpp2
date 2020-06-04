#pragma once
#include "future/span.h"
#include "fwd.h"

namespace xmol::v1 {

/// Storage of atomic data except coords
struct BaseAtom {
  AtomName name;                  /// Atom name
  AtomId id;                      /// Atom id
  BaseResidue* residue = nullptr; /// Parent residue
};

/// Storage of residue data
struct BaseResidue {
  ResidueName name;                 /// Residue name
  ResidueId id;                     /// Residue id
  future::Span<BaseAtom> atoms;     /// Children atoms
  BaseMolecule* molecule = nullptr; /// Parent molecule
};

/// Storage of molecule data
struct BaseMolecule {
  Frame* frame;                       /// Parent frame
  MoleculeName name;                  /// Molecule name
  future::Span<BaseResidue> residues; /// Children residues
};

} // namespace xmol::v1