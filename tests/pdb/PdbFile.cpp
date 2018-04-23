#include <gtest/gtest.h>

#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"


using ::testing::Test;
using namespace xmol::pdb;
using namespace xmol::polymer;



class PdbFileTests : public Test{
public:
};

TEST_F(PdbFileTests,read){
  auto files ={
      "pdb/rcsb/1PGB.pdb",
      "pdb/rcsb/5BMG.pdb",
      "pdb/rcsb/5BMH.pdb",
  };
  for(auto& file: files){
    auto frame = PdbFile(file).get_frame();
    auto atoms= frame.asAtoms();
    auto coords  = frame.asAtoms().toCoords();
    ASSERT_EQ(atoms.size(),coords.size());

    for(int i=0;i<atoms.size();i++){
      ASSERT_DOUBLE_EQ(atoms[i].r().x(),coords[i].x());
      ASSERT_DOUBLE_EQ(atoms[i].r().y(),coords[i].y());
      ASSERT_DOUBLE_EQ(atoms[i].r().z(),coords[i].z());
    }
  }
}
