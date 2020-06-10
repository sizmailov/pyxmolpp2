#include <gtest/gtest.h>

#include "xmol/Frame.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

class ProxyTests : public Test {};

TEST_F(ProxyTests, span_count) {
  auto frame = Frame();

  MoleculeSmartRef m1 = frame.add_molecule().name("A");
  MoleculeSmartRef m2 = frame.add_molecule().name("B");
  MoleculeSmartRef m3 = frame.add_molecule().name("C");
  MoleculeSmartRef m4 = frame.add_molecule().name("D");

  ResidueSmartRef r2 = m3.add_residue().name("one").id(1);
  ResidueSmartRef r3 = m3.add_residue().name("two").id(2);
  ResidueSmartRef r4 = m3.add_residue().name("two").id(3);
  ResidueSmartRef r1 = m2.add_residue().name("thr").id(1);
  ResidueSmartRef r0 = m1.add_residue().name("fou").id(1);

  AtomSmartRef a1 = r3.add_atom().name("X").id(1);
  AtomSmartRef a2 = r2.add_atom().name("Y").id(2);
  AtomSmartRef a3 = r1.add_atom().name("Z").id(2);
  AtomSmartRef a4 = r0.add_atom().name("W").id(2);

  ASSERT_EQ(m1.size(), 1);
  ASSERT_EQ(m1.residues().size(), 1);
  ASSERT_EQ(m1.atoms().size(), 1);

  ASSERT_EQ(m2.size(), 1);
  ASSERT_EQ(m2.residues().size(), 1);
  ASSERT_EQ(m2.atoms().size(), 1);

  ASSERT_EQ(m3.size(), 3);
  ASSERT_EQ(m3.residues().size(), 3);
  ASSERT_EQ(m3.atoms().size(), 2);

  ASSERT_EQ(m4.atoms().size(), 0);
  ASSERT_EQ(m4.residues().size(), 0);

  ASSERT_EQ(&r0.name(), &m1.residues()[0].name());
  ASSERT_EQ(&r1.name(), &m2.residues()[0].name());
  ASSERT_EQ(&r2.name(), &m3.residues()[0].name());
  ASSERT_EQ(&r3.name(), &m3.residues()[1].name());
  ASSERT_EQ(&r4.name(), &m3.residues()[2].name());

  auto atoms = frame.atoms();
  int i = 0;
  for (auto it = atoms.begin(); it != atoms.end(); ++it, ++i) {
    ASSERT_EQ(atoms[i], *it);
  }

  for (MoleculeSmartRef& m : std::array{m1, m2, m3, m4}) {
    for (auto& a : m.atoms()) {
      ASSERT_EQ(&a.molecule().name(), &m.name())
          << a.molecule().name().str() << "." << a.residue().name().str() << "." << a.name().str() << std::endl;
    }
  }
}

TEST_F(ProxyTests, span_conversions) {
  auto frame = Frame();

  EXPECT_EQ(frame.molecules().size(), 0);
  EXPECT_EQ(frame.molecules().residues().size(), 0);
  EXPECT_EQ(frame.molecules().residues().molecules().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 0);

  auto mol = frame.add_molecule();

  EXPECT_EQ(frame.molecules().size(), 1);
  EXPECT_EQ(frame.molecules().residues().size(), 0);
  EXPECT_EQ(frame.molecules().residues().molecules().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 0);

  auto res = mol.add_residue();

  EXPECT_EQ(frame.molecules().size(), 1);
  EXPECT_EQ(frame.molecules().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().molecules().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 0);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 0);

  res.add_atom();

  EXPECT_EQ(frame.molecules().size(), 1);
  EXPECT_EQ(frame.molecules().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 1);

  auto mol2 = frame.add_molecule();

  EXPECT_EQ(frame.molecules().size(), 2);
  EXPECT_EQ(frame.molecules().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().molecules().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 1);

  auto res2 = mol2.add_residue();

  EXPECT_EQ(frame.molecules().size(), 2);
  EXPECT_EQ(frame.molecules().residues().size(), 2);
  EXPECT_EQ(frame.molecules().residues().molecules().size(), 2);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 1);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 1);

  res2.add_atom();

  EXPECT_EQ(frame.molecules().size(), 2);
  EXPECT_EQ(frame.molecules().residues().size(), 2);
  EXPECT_EQ(frame.molecules().residues().atoms().size(), 2);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().size(), 2);
  EXPECT_EQ(frame.molecules().residues().atoms().residues().molecules().size(), 2);
}

TEST_F(ProxyTests, dead_frame_access_from_molecule_ref) {
  auto frame = Frame{};
  MoleculeSmartRef mol = frame.add_molecule();
  MoleculeRef plain_ref = mol;
  frame = {};
  EXPECT_THROW(mol.frame(), DeadFrameAccessError);
  EXPECT_THROW(mol.residues(), DeadFrameAccessError);
  EXPECT_THROW(mol.atoms(), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(mol.name()), DeadFrameAccessError);
  EXPECT_THROW(mol.name({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(mol.size()), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(mol.empty()), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(mol != plain_ref), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(static_cast<MoleculeRef>(mol)), DeadFrameAccessError);
}

TEST_F(ProxyTests, dead_frame_access_from_residue_ref) {
  auto frame = Frame{};
  MoleculeSmartRef mol = frame.add_molecule();
  ResidueSmartRef residue = mol.add_residue();
  ResidueRef plain_ref = residue;
  frame = {};
  EXPECT_THROW(residue.frame(), DeadFrameAccessError);
  EXPECT_THROW(residue.molecule(), DeadFrameAccessError);
  EXPECT_THROW(residue.atoms(), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(residue.name()), DeadFrameAccessError);
  EXPECT_THROW(residue.name({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(residue.id()), DeadFrameAccessError);
  EXPECT_THROW(residue.id({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(residue.size()), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(residue.empty()), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(residue != plain_ref), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(static_cast<ResidueRef>(residue)), DeadFrameAccessError);
}

TEST_F(ProxyTests, dead_frame_access_from_atom_ref) {
  auto frame = Frame{};
  MoleculeSmartRef mol = frame.add_molecule();
  ResidueSmartRef residue = mol.add_residue();
  AtomSmartRef atom = residue.add_atom();
  AtomRef plain_ref = atom;
  frame = {};
  EXPECT_THROW(atom.frame(), DeadFrameAccessError);
  EXPECT_THROW(atom.molecule(), DeadFrameAccessError);
  EXPECT_THROW(atom.residue(), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(atom.name()), DeadFrameAccessError);
  EXPECT_THROW(atom.name({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(atom.id()), DeadFrameAccessError);
  EXPECT_THROW(atom.id({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(atom.r()), DeadFrameAccessError);
  EXPECT_THROW(atom.r({}), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(atom != plain_ref), DeadFrameAccessError);
  EXPECT_THROW(static_cast<void>(static_cast<AtomRef>(atom)), DeadFrameAccessError);
}