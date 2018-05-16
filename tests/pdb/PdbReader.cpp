#include <gtest/gtest.h>

#include "xmol/pdb/PdbReader.h"
#include "xmol/polymer/exceptions.h"


using ::testing::Test;
using namespace xmol::pdb;
using namespace xmol::polymer;



class PdbReaderTests : public Test{
public:
};

TEST_F(PdbReaderTests, sound){
  std::stringstream ss("ATOM     32  N  AARG A  -3Z     11.281  86.699  94.383  0.50 35.88           N  ");
  PdbReader pdbReader(ss);

  auto frame = pdbReader.read_frame();
  auto& chain =  frame.all()[0];
  auto& residue =  chain.all()[0];
  auto& atom =  residue.all()[0];

  EXPECT_EQ(frame.size(),1);
  EXPECT_EQ(chain.size(),1);
  EXPECT_EQ(chain.name(),ChainName("A"));
  EXPECT_EQ(residue.size(),1);
  EXPECT_EQ(residue.name(),ResidueName("ARG"));
  EXPECT_EQ(residue.id().serial,-3);
  EXPECT_EQ(residue.id().iCode,ResidueInsertionCode("Z"));
  EXPECT_EQ(atom.id(),32);
  EXPECT_EQ(atom.name(),AtomName("N"));
  EXPECT_DOUBLE_EQ(atom.r().x(),11.281);
  EXPECT_DOUBLE_EQ(atom.r().y(),86.699);
  EXPECT_DOUBLE_EQ(atom.r().z(),94.383);

  EXPECT_NO_THROW(frame[ChainName("A")]);
  EXPECT_NO_THROW(frame[ChainName("A")][ResidueId(-3,ResidueInsertionCode("Z"))]);
  EXPECT_NO_THROW(frame[ChainName("A")][ResidueId(-3,ResidueInsertionCode("Z"))][AtomName("N")]);
  EXPECT_THROW(frame[ChainName("B")],OutOfRangeFrame);
}
