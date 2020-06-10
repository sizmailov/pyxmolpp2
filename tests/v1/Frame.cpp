#include <gtest/gtest.h>

#include "xmol/Frame.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

class FrameTests : public Test {};

TEST_F(FrameTests, create) {
  auto frame = Frame();

  auto m1 = frame.add_molecule().name("A").smart();
  auto m2 = frame.add_molecule().name("B").smart();
  auto m3 = frame.add_molecule().name("C").smart();

  auto r3 = m3.add_residue().name("GLY").id(1).smart();
  auto r4 = m3.add_residue().name("GLY").id(2).smart();
  auto r2 = m2.add_residue().name("GLY").id(1).smart();
  auto r1 = m1.add_residue().name("GLY").id(1).smart();

  auto a1 = r4.add_atom().name("H").id(1).smart();
  auto a2 = r3.add_atom().name("H").id(2).smart();
  auto a3 = r2.add_atom().name("H").id(2).smart();
  auto a4 = r1.add_atom().name("H").id(2).smart();

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
    auto m1 = frame.add_molecule().name("A").smart();
    auto m2 = frame.add_molecule().name("B").smart();
    auto m3 = frame.add_molecule().name("C").smart();
    {
      auto r3 = m3.add_residue().name("GLY").id(1).smart();
      auto r4 = m3.add_residue().name("GLY").id(2).smart();
      auto r2 = m2.add_residue().name("GLY").id(1).smart();
      auto r1 = m1.add_residue().name("GLY").id(1).smart();
      {
        auto a1 = r4.add_atom().name("X").id(1).smart();
        auto a2 = r3.add_atom().name("Y").id(2).smart();
        auto a3 = r2.add_atom().name("Z").id(2).smart();
        auto a4 = r1.add_atom().name("W").id(2).smart();

        frame2 = std::move(frame);

        ASSERT_EQ(frame.n_molecules(), 0);
        ASSERT_EQ(frame.n_residues(), 0);
        ASSERT_EQ(frame.n_atoms(), 0);

        ASSERT_EQ(frame.n_references<MoleculeSmartRef>(), 0);
        ASSERT_EQ(frame.n_references<ResidueSmartRef>(), 0);
        ASSERT_EQ(frame.n_references<AtomSmartRef>(), 0);

        ASSERT_EQ(frame2.n_molecules(), 3);
        ASSERT_EQ(frame2.n_residues(), 4);
        ASSERT_EQ(frame2.n_atoms(), 4);

        ASSERT_EQ(frame2.n_references<MoleculeSmartRef>(), 3);
        ASSERT_EQ(frame2.n_references<ResidueSmartRef>(), 4);
        ASSERT_EQ(frame2.n_references<AtomSmartRef>(), 4);

        ASSERT_EQ(a1.name(), AtomName("X"));
        ASSERT_EQ(a2.name(), AtomName("Y"));
        ASSERT_EQ(a3.name(), AtomName("Z"));
        ASSERT_EQ(a4.name(), AtomName("W"));
      }

      ASSERT_EQ(frame2.n_references<MoleculeSmartRef>(), 3);
      ASSERT_EQ(frame2.n_references<ResidueSmartRef>(), 4);
      ASSERT_EQ(frame2.n_references<AtomSmartRef>(), 0);
    }

    ASSERT_EQ(frame2.n_references<MoleculeSmartRef>(), 3);
    ASSERT_EQ(frame2.n_references<ResidueSmartRef>(), 0);
    ASSERT_EQ(frame2.n_references<AtomSmartRef>(), 0);
  }

  ASSERT_EQ(frame2.n_references<MoleculeSmartRef>(), 0);
  ASSERT_EQ(frame2.n_references<ResidueSmartRef>(), 0);
  ASSERT_EQ(frame2.n_references<AtomSmartRef>(), 0);
}

TEST_F(FrameTests, frame_move_construction) {
  Frame frame;
  auto m1 = frame.add_molecule().name("A").smart();
  auto m2 = frame.add_molecule().name("B").smart();
  auto m3 = frame.add_molecule().name("C").smart();
  auto r3 = m3.add_residue().name("GLY").id(1).smart();
  auto r4 = m3.add_residue().name("GLY").id(2).smart();
  auto r2 = m2.add_residue().name("GLY").id(1).smart();
  auto r1 = m1.add_residue().name("GLY").id(1).smart();
  auto a1 = r4.add_atom().name("X").id(1).smart();
  auto a2 = r3.add_atom().name("Y").id(2).smart();
  auto a3 = r2.add_atom().name("Z").id(2).smart();
  auto a4 = r1.add_atom().name("W").id(2).smart();

  auto frame2 = std::move(frame);

  ASSERT_EQ(frame.n_molecules(), 0);
  ASSERT_EQ(frame.n_residues(), 0);
  ASSERT_EQ(frame.n_atoms(), 0);

  ASSERT_EQ(frame.n_references<MoleculeSmartRef>(), 0);
  ASSERT_EQ(frame.n_references<ResidueSmartRef>(), 0);
  ASSERT_EQ(frame.n_references<AtomSmartRef>(), 0);

  ASSERT_EQ(frame2.n_molecules(), 3);
  ASSERT_EQ(frame2.n_residues(), 4);
  ASSERT_EQ(frame2.n_atoms(), 4);

  ASSERT_EQ(frame2.n_references<MoleculeSmartRef>(), 3);
  ASSERT_EQ(frame2.n_references<ResidueSmartRef>(), 4);
  ASSERT_EQ(frame2.n_references<AtomSmartRef>(), 4);

  ASSERT_EQ(a1.name(), AtomName("X"));
  ASSERT_EQ(a2.name(), AtomName("Y"));
  ASSERT_EQ(a3.name(), AtomName("Z"));
  ASSERT_EQ(a4.name(), AtomName("W"));
}

TEST_F(FrameTests, molecule_construction) {
  Frame frame;
  auto mol = frame.add_molecule().name("X").smart();
  ASSERT_TRUE(mol.empty());
  ASSERT_EQ(mol.name(), MoleculeName("X"));
  mol.name(MoleculeName("Y"));
  ASSERT_EQ(mol.name(), MoleculeName("Y"));
  {
    auto mol_ref_copy = mol;
    ASSERT_EQ(frame.n_references<MoleculeSmartRef>(), 2);
    auto mol_ref_moved = std::move(mol);
    ASSERT_EQ(frame.n_references<MoleculeSmartRef>(), 2);
    static_cast<void>(mol_ref_copy);
    static_cast<void>(mol_ref_moved);
  }
  ASSERT_EQ(frame.n_references<MoleculeSmartRef>(), 0);
}

TEST_F(FrameTests, residue_construction) {
  Frame frame;
  auto mol = frame.add_molecule();
  auto residue = mol.add_residue().name("ABC").id(1);
  ASSERT_TRUE(residue.empty());
  ASSERT_EQ(residue.name(), ResidueName("ABC"));
  residue.name(ResidueName("ZYX"));
  ASSERT_EQ(residue.name(), ResidueName("ZYX"));
}

TEST_F(FrameTests, frame_backward_construction_1) {
  Frame frame;
  auto mol = frame.add_molecule().name("A").smart();
  auto res1 = mol.add_residue().name("A1").id(1).smart();
  auto res2 = mol.add_residue().name("A2").id(2).smart();
  int n = 10;
  for (int i = 0; i < n; ++i)
    res2.add_atom().name("R1").id(1);
  mol.add_residue().name("A3").id(2);
  for (int i = 0; i < n; ++i)
    res1.add_atom().name("R2").id(1);

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
    molecules.emplace_back(frame.add_molecule());
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.emplace_back(molecules[i].add_residue());
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.emplace_back(it->add_atom());
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
    auto molecule = frame.add_molecule().smart();
    molecules.push_back(molecule);
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      auto residue = molecule.add_residue().smart();
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        atoms.emplace_back(residue.add_atom());
      }
    }
  }

  for (int i = n_molecules - 1; i >= 0; --i) {
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      residues.emplace_back(molecules[i].add_residue());
    }
  }

  for (auto it = residues.rbegin(); it != residues.rend(); ++it) {
    for (int j = 0; j < n_atoms_per_residue; ++j) {
      atoms.emplace_back(it->add_atom());
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
    auto molecule = frame.add_molecule().smart();
    for (int j = 0; j < n_residues_per_molecule; ++j) {
      auto residue = molecule.add_residue().smart();
      for (int k = 0; k < n_atoms_per_residue; ++k) {
        residue.add_atom();
      }
    }
  }

  ASSERT_EQ(frame.n_molecules(), n_molecules);
  ASSERT_EQ(frame.n_residues(), n_molecules * n_residues_per_molecule);
  ASSERT_EQ(frame.n_atoms(), n_molecules * n_residues_per_molecule * n_atoms_per_residue);
}
