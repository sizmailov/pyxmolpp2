#include <gtest/gtest.h>

#include "xmol/geometry/alignment.h"


using ::testing::Test;
using namespace xmol::geometry;

class alignmentTests : public Test{
public:
};


TEST_F(alignmentTests, align){

  Transformation3d G = Translation3d(XYZ(-1,1,2))*Rotation3d(XYZ(1,2,3),Degrees(10));

  std::vector<XYZ> reference = {XYZ(1,2,3),XYZ(2,4,5),XYZ(8,1,3),XYZ(1,1,1),XYZ(5,1,2)};
  std::vector<XYZ> variable = reference;

  for (auto&x: variable){x = G.transform(x);}

  EXPECT_GE(calc_rmsd(reference,variable),1);

  auto G2 = calc_alignment(reference,variable);

  EXPECT_LE(calc_rmsd(reference,variable,G2),1e-9);

  for (auto&x: variable){x = G2.transform(x);}
  EXPECT_LE(calc_rmsd(reference,variable),1e-9);

}
