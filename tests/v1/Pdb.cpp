#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"
#include "xmol/v1/io/PdbInputFile.h"

using ::testing::Test;
using namespace xmol::v1;
using namespace xmol::v1::proxy;
using namespace xmol::v1::proxy::smart;
using namespace xmol::v1::io;

class PdbTests : public Test {};

TEST_F(PdbTests, read) {
  const std::string filename("trjtool/GB1/run00001.pdb");
  Frame frame = PdbInputFile(filename, PdbInputFile::Dialect::AMBER_99).frames()[0];
  EXPECT_EQ(frame.n_atoms(), 880);
  EXPECT_EQ(frame.n_residues(), 56);
  for (auto&& mol: frame.molecules()){
    std::cout << mol.name().str() << " " << mol.size() << std::endl;
  }
}