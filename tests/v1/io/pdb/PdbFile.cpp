#include <gtest/gtest.h>

#include "xmol/io/PdbInputFile.h"
#include "xmol/io/pdb/PdbReader.h"

using ::testing::Test;
using namespace xmol::io;
using namespace xmol;

class PdbFileTests : public Test {
public:
};

TEST_F(PdbFileTests, read) {
  auto files = {
      "pdb/rcsb/1PGB.pdb",
      "pdb/rcsb/5BMG.pdb",
      "pdb/rcsb/5BMH.pdb",
  };
  for (auto& file : files) {
    auto frame = PdbInputFile(file).frames()[0];
    auto atoms = frame.atoms();
    auto coords = frame.coords();

    ASSERT_EQ(atoms.size(), coords.size());
    for (int i = 0; i < atoms.size(); i++) {
      ASSERT_EQ(atoms[i].r().x(), coords[i].x());
      ASSERT_EQ(atoms[i].r().y(), coords[i].y());
      ASSERT_EQ(atoms[i].r().z(), coords[i].z());
    }
  }
}
