#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"

using ::testing::Test;
using namespace xmol::v1; 

class ProxyTests : public Test {};

TEST_F(ProxyTests, span_count) {
  auto frame = Frame();

  MoleculeSmartRef m1 = frame.add_molecule(MoleculeName("A"));
  MoleculeSmartRef m2 = frame.add_molecule(MoleculeName("B"));
  MoleculeSmartRef m3 = frame.add_molecule(MoleculeName("C"));
  MoleculeSmartRef m4 = frame.add_molecule(MoleculeName("D"));

  ResidueSmartRef r2 = m3.add_residue(ResidueName("one"), ResidueId(1));
  ResidueSmartRef r3 = m3.add_residue(ResidueName("two"), ResidueId(2));
  ResidueSmartRef r4 = m3.add_residue(ResidueName("two"), ResidueId(3));
  ResidueSmartRef r1 = m2.add_residue(ResidueName("thr"), ResidueId(1));
  ResidueSmartRef r0 = m1.add_residue(ResidueName("fou"), ResidueId(1));

  AtomSmartRef a1 = r3.add_atom(AtomName("X"), AtomId(1));
  AtomSmartRef a2 = r2.add_atom(AtomName("Y"), AtomId(2));
  AtomSmartRef a3 = r1.add_atom(AtomName("Z"), AtomId(2));
  AtomSmartRef a4 = r0.add_atom(AtomName("W"), AtomId(2));

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