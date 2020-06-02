#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"

using ::testing::Test;
using namespace xmol::v1;

class FrameTests : public Test {};

TEST_F(FrameTests, create) {
  auto frame = Frame();

  auto m1 = frame.add_molecule(MoleculeName("A"));
  auto m2 = frame.add_molecule(MoleculeName("B"));
  auto m3 = frame.add_molecule(MoleculeName("C"));

  auto r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
  auto r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
  auto r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
  auto r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));

  auto a1 = r4.add_atom(AtomName("H"), AtomId(1));
  auto a2 = r3.add_atom(AtomName("H"), AtomId(2));
  auto a3 = r2.add_atom(AtomName("H"), AtomId(2));
  auto a4 = r1.add_atom(AtomName("H"), AtomId(2));

  ASSERT_EQ(m1.size(), 1);
  ASSERT_EQ(m2.size(), 1);
  ASSERT_EQ(m3.size(), 2);

  ASSERT_EQ(r1.size(), 1);
  ASSERT_EQ(r2.size(), 1);
  ASSERT_EQ(r3.size(), 1);
  ASSERT_EQ(r4.size(), 1);
}

TEST_F(FrameTests, frame_move_assignement) {
  Frame frame;
  Frame frame2;
  {
    auto m1 = frame.add_molecule(MoleculeName("A"));
    auto m2 = frame.add_molecule(MoleculeName("B"));
    auto m3 = frame.add_molecule(MoleculeName("C"));
    {
      auto r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
      auto r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
      auto r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
      auto r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));
      {
        auto a1 = r4.add_atom(AtomName("X"), AtomId(1));
        auto a2 = r3.add_atom(AtomName("Y"), AtomId(2));
        auto a3 = r2.add_atom(AtomName("Z"), AtomId(2));
        auto a4 = r1.add_atom(AtomName("W"), AtomId(2));

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
  auto m1 = frame.add_molecule(MoleculeName("A"));
  auto m2 = frame.add_molecule(MoleculeName("B"));
  auto m3 = frame.add_molecule(MoleculeName("C"));
  auto r3 = m3.add_residue(ResidueName("GLY"), ResidueId(1));
  auto r4 = m3.add_residue(ResidueName("GLY"), ResidueId(2));
  auto r2 = m2.add_residue(ResidueName("GLY"), ResidueId(1));
  auto r1 = m1.add_residue(ResidueName("GLY"), ResidueId(1));
  auto a1 = r4.add_atom(AtomName("X"), AtomId(1));
  auto a2 = r3.add_atom(AtomName("Y"), AtomId(2));
  auto a3 = r2.add_atom(AtomName("Z"), AtomId(2));
  auto a4 = r1.add_atom(AtomName("W"), AtomId(2));

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
  auto mol = frame.add_molecule(MoleculeName("X"));
  ASSERT_TRUE(mol.empty());
  ASSERT_EQ(mol.name(), MoleculeName("X"));
  mol.name(MoleculeName("Y"));
  ASSERT_EQ(mol.name(), MoleculeName("Y"));
  {
    auto mol_ref_copy = mol;
    ASSERT_EQ(frame.n_molecule_references(), 2);
    auto mol_ref_moved = std::move(mol);
    ASSERT_EQ(frame.n_molecule_references(), 2);
  }
  ASSERT_EQ(frame.n_molecule_references(), 0);
}

TEST_F(FrameTests, residue_construction) {
  Frame frame;
  auto mol = frame.add_molecule(MoleculeName("X"));
  auto residue = mol.add_residue(ResidueName("ABC"), ResidueId(1));
  ASSERT_TRUE(residue.empty());
  ASSERT_EQ(residue.name(), ResidueName("ABC"));
  residue.name(ResidueName("ZYX"));
  ASSERT_EQ(residue.name(), ResidueName("ZYX"));
}

TEST_F(FrameTests, frame_backward_construction_1) {
  Frame frame;
  auto mol = frame.add_molecule(MoleculeName("A"));
  auto res1 = mol.add_residue(ResidueName("A1"), ResidueId(1));
  auto res2 = mol.add_residue(ResidueName("A2"), ResidueId(2));
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

  std::vector<MoleculeRef> molecules;
  std::vector<ResidueRef> residues;
  std::vector<AtomRef> atoms;

  const int n_molecules = 13;
  const int n_residues_per_molecule = 11;
  const int n_atoms_per_residue = 10;

  for (int i = 0; i < n_molecules; ++i) {
    molecules.push_back(frame.add_molecule(MoleculeName("")));
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.push_back(molecules[i].add_residue(ResidueName(""), ResidueId(0)));
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.push_back(it->add_atom(AtomName(""), AtomId(0)));
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue);
}

TEST_F(FrameTests, frame_backward_construction_3) {
  Frame frame;

  std::vector<MoleculeRef> molecules;
  std::vector<ResidueRef> residues;
  std::vector<AtomRef> atoms;

  const int n_molecules = 13;
  const int n_residues_per_molecule = 11;
  const int n_atoms_per_residue = 7;

  for (int i = 0; i < n_molecules; ++i) {
    MoleculeRef molecule = frame.add_molecule(MoleculeName(""));
    molecules.push_back(molecule);
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      ResidueRef residue = molecule.add_residue(ResidueName(""), ResidueId(0));
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        atoms.push_back(residue.add_atom(AtomName(""), AtomId(0)));
      }
    }
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.push_back(molecules[i].add_residue(ResidueName(""), ResidueId(0)));
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.push_back(it->add_atom(AtomName(""), AtomId(0)));
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
    MoleculeRef molecule = frame.add_molecule(MoleculeName(""));
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      ResidueRef residue = molecule.add_residue(ResidueName(""), ResidueId(0));
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        residue.add_atom(AtomName(""), AtomId(0));
      }
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue);
}
