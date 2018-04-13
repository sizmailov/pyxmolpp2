#include <gtest/gtest.h>

#include "xmol/trjtool/DATReader.h"
#include "xmol/pdb/PDBReader.h"
#include <fstream>


using ::testing::Test;
using namespace xmol::trjtool;



class TrjtoolTests : public Test{
public:
};

TEST_F(TrjtoolTests, read){
//  std::ifstream ifs2("/home/sergei/bionmr/seva/GB1/5bmg/5bmg_CML/5_run/run00010.pdb",std::ios::binary);
//  auto frame = xmol::pdb::PDBReader(ifs2).read_frame();
//  std::ifstream ifs("/home/sergei/bionmr/seva/GB1/5bmg/5bmg_CML/5_run/run00010.dat",std::ios::binary);
//
//  DATReader reader(ifs);
//
//
//  auto com = [](const xmol::polymer::AtomSelection& sel){
//    xmol::polymer::XYZ center{0,0,0};
//    for (auto&a: sel){
//      center.x+=a.r().x;
//      center.y+=a.r().y;
//      center.z+=a.r().z;
//    }
//    center.x/=sel.size();
//    center.y/=sel.size();
//    center.z/=sel.size();
//    return center;
//  };
//
//  auto atoms = frame.asAtoms();
//  EXPECT_TRUE(reader.match(atoms));
//
//  for (int i=0;i<reader.n_frames();i++){
//    reader.set_frame(i,atoms);
//    auto c = com(atoms);
//    std::cout
//        << i << "\t"
//        << c.x << "\t "
//        << c.y << "\t "
//        << c.z << "\t "
//                        << std::endl;
//  }
}
