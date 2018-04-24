#include <gtest/gtest.h>

#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbWriter.h"


using ::testing::Test;
using namespace xmol::pdb;
using namespace xmol::polymer;



class PdbWriterTests : public Test{
public:
};


TEST_F(PdbWriterTests, write){
  std::string pdb_line = "ATOM     32  N  AARG A  -3      11.281  86.699  94.383  0.50 35.88           N  ";
  std::stringstream ss(pdb_line);
  PdbReader pdbReader(ss);

  auto frame = pdbReader.read_frame();
  auto& chain =  frame.all()[0];
  auto& residue =  chain.all()[0];
  auto& atom =  residue.all()[0];


  std::stringstream ss2;
  PdbWriter writer(ss2);
  writer.write(frame);

  auto frame2 = PdbReader(ss2).read_frame();
  auto& chain2 =  frame.all()[0];
  auto& residue2 =  chain.all()[0];
  auto& atom2 =  residue.all()[0];


  EXPECT_EQ(frame.size(),frame2.size());
  EXPECT_EQ(chain.size(),chain2.size());
  EXPECT_EQ(chain.name(),chain2.name());
  EXPECT_EQ(residue.size(),residue2.size());
  EXPECT_EQ(residue.name(),residue2.name());
  EXPECT_EQ(residue.id(),residue2.id());
  EXPECT_EQ(atom.id(),atom2.id());
  EXPECT_EQ(atom.name(),atom2.name());
  EXPECT_DOUBLE_EQ(atom.r().x(),atom2.r().x());
  EXPECT_DOUBLE_EQ(atom.r().y(),atom2.r().y());
  EXPECT_DOUBLE_EQ(atom.r().z(),atom2.r().z());


}