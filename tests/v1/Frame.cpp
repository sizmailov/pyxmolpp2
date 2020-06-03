#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"

using ::testing::Test;
using namespace xmol::v1;

class FrameTests : public Test {};

TEST_F(FrameTests, create) {
  auto frame = Frame();

  MoleculeSmartRef m1 = frame.add_molecule(MoleculeName("A"));
  MoleculeSmartRef m2 = frame.add_molecule(MoleculeName("B"));
  MoleculeSmartRef m3 = frame.add_molecule(MoleculeName("C"));

  ResidueSmartRef r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
  ResidueSmartRef r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
  ResidueSmartRef r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
  ResidueSmartRef r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));

  AtomSmartRef a1 = r4.add_atom(AtomName("H"), AtomId(1));
  AtomSmartRef a2 = r3.add_atom(AtomName("H"), AtomId(2));
  AtomSmartRef a3 = r2.add_atom(AtomName("H"), AtomId(2));
  AtomSmartRef a4 = r1.add_atom(AtomName("H"), AtomId(2));

  ASSERT_EQ(m1.size(), 1);
  ASSERT_EQ(m2.size(), 1);
  ASSERT_EQ(m3.size(), 2);

  ASSERT_EQ(r1.size(), 1);
  ASSERT_EQ(r2.size(), 1);
  ASSERT_EQ(r3.size(), 1);
  ASSERT_EQ(r4.size(), 1);

  ASSERT_EQ(a1.name(), AtomName("H"));
  ASSERT_EQ(a2.name(), AtomName("H"));
  ASSERT_EQ(a3.name(), AtomName("H"));
  ASSERT_EQ(a4.name(), AtomName("H"));

}

TEST_F(FrameTests, frame_move_assignement) {
  Frame frame;
  Frame frame2;
  {
    MoleculeSmartRef m1 = frame.add_molecule(MoleculeName("A"));
    MoleculeSmartRef m2 = frame.add_molecule(MoleculeName("B"));
    MoleculeSmartRef m3 = frame.add_molecule(MoleculeName("C"));
    {
      ResidueSmartRef r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
      ResidueSmartRef r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
      ResidueSmartRef r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
      ResidueSmartRef r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));
      {
        AtomSmartRef a1 = r4.add_atom(AtomName("X"), AtomId(1));
        AtomSmartRef a2 = r3.add_atom(AtomName("Y"), AtomId(2));
        AtomSmartRef a3 = r2.add_atom(AtomName("Z"), AtomId(2));
        AtomSmartRef a4 = r1.add_atom(AtomName("W"), AtomId(2));

        frame2 = std::move(frame);

        ASSERT_EQ(frame.n_molecules(), 0);
        ASSERT_EQ(frame.n_residues(), 0);
        ASSERT_EQ(frame.n_atoms(), 0);

        ASSERT_EQ(frame.n_molecule_references(), 0);
        ASSERT_EQ(frame.n_residue_references(), 0);
        ASSERT_EQ(frame.n_atom_references(), 0);

        ASSERT_EQ(frame2.n_molecules(), 3);
        ASSERT_EQ(frame2.n_residues(), 4);
        ASSERT_EQ(frame2.n_atoms(), 4);

        ASSERT_EQ(frame2.n_molecule_references(), 3);
        ASSERT_EQ(frame2.n_residue_references(), 4);
        ASSERT_EQ(frame2.n_atom_references(), 4);

        ASSERT_EQ(a1.name(), AtomName("X"));
        ASSERT_EQ(a2.name(), AtomName("Y"));
        ASSERT_EQ(a3.name(), AtomName("Z"));
        ASSERT_EQ(a4.name(), AtomName("W"));
      }

      ASSERT_EQ(frame2.n_molecule_references(), 3);
      ASSERT_EQ(frame2.n_residue_references(), 4);
      ASSERT_EQ(frame2.n_atom_references(), 0);
    }

    ASSERT_EQ(frame2.n_molecule_references(), 3);
    ASSERT_EQ(frame2.n_residue_references(), 0);
    ASSERT_EQ(frame2.n_atom_references(), 0);
  }

  ASSERT_EQ(frame2.n_molecule_references(), 0);
  ASSERT_EQ(frame2.n_residue_references(), 0);
  ASSERT_EQ(frame2.n_atom_references(), 0);
}

TEST_F(FrameTests, frame_move_construction) {
  Frame frame;
  MoleculeSmartRef m1 = frame.add_molecule(MoleculeName("A"));
  MoleculeSmartRef m2 = frame.add_molecule(MoleculeName("B"));
  MoleculeSmartRef m3 = frame.add_molecule(MoleculeName("C"));
  ResidueSmartRef r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
  ResidueSmartRef r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
  ResidueSmartRef r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
  ResidueSmartRef r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));
  AtomSmartRef a1 = r4.add_atom(AtomName("X"), AtomId(1));
  AtomSmartRef a2 = r3.add_atom(AtomName("Y"), AtomId(2));
  AtomSmartRef a3 = r2.add_atom(AtomName("Z"), AtomId(2));
  AtomSmartRef a4 = r1.add_atom(AtomName("W"), AtomId(2));

  auto frame2 = std::move(frame);

  ASSERT_EQ(frame.n_molecules(), 0);
  ASSERT_EQ(frame.n_residues(), 0);
  ASSERT_EQ(frame.n_atoms(), 0);

  ASSERT_EQ(frame.n_molecule_references(), 0);
  ASSERT_EQ(frame.n_residue_references(), 0);
  ASSERT_EQ(frame.n_atom_references(), 0);

  ASSERT_EQ(frame2.n_molecules(), 3);
  ASSERT_EQ(frame2.n_residues(), 4);
  ASSERT_EQ(frame2.n_atoms(), 4);

  ASSERT_EQ(frame2.n_molecule_references(), 3);
  ASSERT_EQ(frame2.n_residue_references(), 4);
  ASSERT_EQ(frame2.n_atom_references(), 4);

  ASSERT_EQ(a1.name(), AtomName("X"));
  ASSERT_EQ(a2.name(), AtomName("Y"));
  ASSERT_EQ(a3.name(), AtomName("Z"));
  ASSERT_EQ(a4.name(), AtomName("W"));
}

TEST_F(FrameTests, molecule_construction) {
  Frame frame;
  MoleculeSmartRef mol = frame.add_molecule(MoleculeName("X"));
  ASSERT_TRUE(mol.empty());
  ASSERT_EQ(mol.name(), MoleculeName("X"));
  mol.name(MoleculeName("Y"));
  ASSERT_EQ(mol.name(), MoleculeName("Y"));
  {
    MoleculeSmartRef mol_ref_copy = mol;
    ASSERT_EQ(frame.n_molecule_references(), 2);
    MoleculeSmartRef mol_ref_moved = std::move(mol);
    ASSERT_EQ(frame.n_molecule_references(), 2);
    static_cast<void>(mol_ref_copy);
    static_cast<void>(mol_ref_moved);
  }
  ASSERT_EQ(frame.n_molecule_references(), 0);
}

TEST_F(FrameTests, residue_construction) {
  Frame frame;
  MoleculeSmartRef mol = frame.add_molecule(MoleculeName("X"));
  ResidueSmartRef residue = mol.add_residue(ResidueName("ABC"), ResidueId(1));
  ASSERT_TRUE(residue.empty());
  ASSERT_EQ(residue.name(), ResidueName("ABC"));
  residue.name(ResidueName("ZYX"));
  ASSERT_EQ(residue.name(), ResidueName("ZYX"));
}

TEST_F(FrameTests, frame_backward_construction_1) {
  Frame frame;
  MoleculeSmartRef mol = frame.add_molecule(MoleculeName("A"));
  ResidueSmartRef res1 = mol.add_residue(ResidueName("A1"), ResidueId(1));
  ResidueSmartRef res2 = mol.add_residue(ResidueName("A2"), ResidueId(2));
  int n = 10;
  for (int i = 0; i < n; ++i)
    res2.add_atom(AtomName("R1"), AtomId(1));
  mol.add_residue(ResidueName("A3"), ResidueId(2));
  for (int i = 0; i < n; ++i)
    res1.add_atom(AtomName("R2"), AtomId(1));

  ASSERT_EQ(frame.n_atoms(), n * 2);
  ASSERT_EQ(frame.n_residues(), 3);
  ASSERT_EQ(frame.n_molecules(), 1);
}

TEST_F(FrameTests, frame_backward_construction_2) {
  Frame frame;

  std::vector<MoleculeSmartRef> molecules;
  std::vector<ResidueSmartRef> residues;
  std::vector<AtomSmartRef> atoms;

  const int n_molecules = 13;
  const int n_residues_per_molecule = 11;
  const int n_atoms_per_residue = 10;

  for (int i = 0; i < n_molecules; ++i) {
    molecules.emplace_back(frame.add_molecule(MoleculeName("")));
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.emplace_back(molecules[i].add_residue(ResidueName(""), ResidueId(0)));
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.emplace_back(it->add_atom(AtomName(""), AtomId(0)));
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue);
}

TEST_F(FrameTests, frame_backward_construction_3) {
  Frame frame;

  std::vector<MoleculeSmartRef> molecules;
  std::vector<ResidueSmartRef> residues;
  std::vector<AtomSmartRef> atoms;

  const int n_molecules = 13;
  const int n_residues_per_molecule = 11;
  const int n_atoms_per_residue = 7;

  for (int i = 0; i < n_molecules; ++i) {
    MoleculeSmartRef molecule = frame.add_molecule(MoleculeName(""));
    molecules.push_back(molecule);
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      ResidueSmartRef residue = molecule.add_residue(ResidueName(""), ResidueId(0));
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        atoms.emplace_back(residue.add_atom(AtomName(""), AtomId(0)));
      }
    }
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.emplace_back(molecules[i].add_residue(ResidueName(""), ResidueId(0)));
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.emplace_back(it->add_atom(AtomName(""), AtomId(0)));
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule * 2);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue * 2);
}

TEST_F(FrameTests, frame_forward_construction) {
  Frame frame;

  const int n_molecules = 13;
  const int n_residues_per_molecule = 17;
  const int n_atoms_per_residue = 11;

  frame.reserve_molecules(n_molecules);
  frame.reserve_residues(n_molecules * n_residues_per_molecule);
  frame.reserve_atoms(n_molecules * n_residues_per_molecule * n_atoms_per_residue);

  for (int i = 0; i < n_molecules; ++i) {
    MoleculeSmartRef molecule = frame.add_molecule(MoleculeName(""));
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      ResidueSmartRef residue = molecule.add_residue(ResidueName(""), ResidueId(0));
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        residue.add_atom(AtomName(""), AtomId(0));
      }
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue);
}
