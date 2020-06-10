#include <gtest/gtest.h>

#include "xmol/io/pdb/PdbReader.h"
#include "xmol/io/pdb/PdbWriter.h"

using ::testing::Test;
using namespace xmol::io::pdb;
using namespace xmol;

class PdbWriterTests : public Test {
public:
};

TEST_F(PdbWriterTests, write) {
  std::string pdb_line = "ATOM     32  N  AARG A  -3      11.281  86.699  94.383  0.50 35.88           N  ";
  std::stringstream ss(pdb_line);
  PdbReader pdbReader(ss);

  auto frame = pdbReader.read_frame();
  auto chain = frame.molecules()[0];
  auto residue = frame.residues()[0];
  auto atom = frame.atoms()[0];

  std::stringstream ss2;
  PdbWriter writer(ss2);
  writer.write(frame);

  auto frame2 = PdbReader(ss2).read_frame();
  auto chain2 = frame2.molecules()[0];
  auto residue2 = frame2.residues()[0];
  auto atom2 = frame2.atoms()[0];

  EXPECT_EQ(frame.n_molecules(), frame2.n_molecules());
  EXPECT_EQ(chain.size(), chain2.size());
  EXPECT_EQ(chain.name(), chain2.name());
  EXPECT_EQ(residue.size(), residue2.size());
  EXPECT_EQ(residue.name(), residue2.name());
  EXPECT_EQ(residue.id(), residue2.id());
  EXPECT_EQ(atom.id(), atom2.id());
  EXPECT_EQ(atom.name(), atom2.name());
  EXPECT_DOUBLE_EQ(atom.r().x(), atom2.r().x());
  EXPECT_DOUBLE_EQ(atom.r().y(), atom2.r().y());
  EXPECT_DOUBLE_EQ(atom.r().z(), atom2.r().z());
}