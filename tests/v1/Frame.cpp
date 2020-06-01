#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"

using ::testing::Test;
using namespace xmol::v1;

class FrameTests : public Test {};

TEST_F(FrameTests, create_molecule) {
  auto frame = Frame();
  auto m1 = frame.add_molecule(ChainName("A"));
  auto m2 = frame.add_molecule(ChainName("B"));
  auto m3 = frame.add_molecule(ChainName("C"));

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