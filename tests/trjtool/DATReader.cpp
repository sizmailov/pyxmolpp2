#include <gtest/gtest.h>

#include "xmol/trjtool/DATReader.h"
#include "xmol/pdb/PDBReader.h"
#include <fstream>


using ::testing::Test;
using namespace xmol::trjtool;



class TrjtoolTests : public Test{
public:
};

TEST_F(TrjtoolTests, basic_read){
  std::ifstream ifs2("trjtool/GB1/run00001.pdb",std::ios::binary);
  auto frame = xmol::pdb::PDBReader(ifs2).read_frame();
  std::ifstream ifs("trjtool/GB1/run00001.dat",std::ios::binary);

  DATReader reader(ifs);

  auto atoms = frame.asAtoms();
  EXPECT_TRUE(reader.match(atoms));
  EXPECT_EQ(reader.n_atoms_per_frame(),frame.asAtoms().size());
  EXPECT_EQ(reader.n_frames(),1000);

}
