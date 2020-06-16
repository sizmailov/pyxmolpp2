#include <gtest/gtest.h>

#include "xmol/io/pdb/PdbReader.h"


using ::testing::Test;
using namespace xmol::io::pdb;
using namespace xmol;



class PdbReaderTests : public Test{
public:
};

TEST_F(PdbReaderTests, sound){
  std::stringstream ss("ATOM     32  N  AARG A  -3Z     11.281  86.699  94.383  0.50 35.88           N  ");
  PdbReader pdbReader(ss);

  auto frame = pdbReader.read_frame();
  auto chain = frame.molecules()[0];
  auto residue = frame.residues()[0];
  auto atom = frame.atoms()[0];

  EXPECT_EQ(frame.n_molecules(), 1);
  EXPECT_EQ(chain.size(), 1);
  EXPECT_EQ(chain.name(), MoleculeName("A"));
  EXPECT_EQ(residue.size(), 1);
  EXPECT_EQ(residue.name(), ResidueName("ARG"));
  EXPECT_EQ(residue.id().serial, -3);
  EXPECT_EQ(residue.id().iCode, ResidueInsertionCode("Z"));
  EXPECT_EQ(atom.id(), 32);
  EXPECT_EQ(atom.name(), AtomName("N"));
  EXPECT_DOUBLE_EQ(atom.r().x(), 11.281);
  EXPECT_DOUBLE_EQ(atom.r().y(), 86.699);
  EXPECT_DOUBLE_EQ(atom.r().z(), 94.383);

  EXPECT_NO_THROW(frame["A"]);
  EXPECT_NO_THROW(frame["A"].value()[ResidueId(-3,ResidueInsertionCode("Z"))].value());
  EXPECT_NO_THROW(frame["A"].value()[ResidueId(-3,ResidueInsertionCode("Z"))].value()[AtomName("N")].value());
  EXPECT_THROW(frame["B"].value(), std::bad_optional_access);
}
