#include "common.h"

void populate_frame(Frame& frame, int n_molecules, int n_residues, int n_atoms) {
  int aid = 1;
  int rid = 1;
  for (int i = 0; i < n_molecules; ++i) {
    auto mol = frame.add_molecule().name("A");
    for (int j = 0; j < n_residues; ++j) {
      auto r = mol.add_residue().name("GLY").id(rid++);
      for (int k = 0; k < n_atoms; ++k) {
        r.add_atom().name("N").id(aid++);
      }
    }
  }
}