#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy-span-impl.h"

using ::testing::Test;
using namespace xmol::v1;

class ProxyTests : public Test {};

TEST_F(ProxyTests, span_count) {
  auto frame = Frame();

  auto m1 = frame.add_molecule(MoleculeName("A"));
  auto m2 = frame.add_molecule(MoleculeName("B"));
  auto m3 = frame.add_molecule(MoleculeName("C"));
  auto m4 = frame.add_molecule(MoleculeName("D"));

  auto r2 = m3.add_residue(ResidueName("one"), ResidueId(1));
  auto r3 = m3.add_residue(ResidueName("two"), ResidueId(2));
  auto r4 = m3.add_residue(ResidueName("two"), ResidueId(3));
  auto r1 = m2.add_residue(ResidueName("thr"), ResidueId(1));
  auto r0 = m1.add_residue(ResidueName("fou"), ResidueId(1));

  auto a1 = r3.add_atom(AtomName("X"), AtomId(1));
  auto a2 = r2.add_atom(AtomName("Y"), AtomId(2));
  auto a3 = r1.add_atom(AtomName("Z"), AtomId(2));
  auto a4 = r0.add_atom(AtomName("W"), AtomId(2));

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

}