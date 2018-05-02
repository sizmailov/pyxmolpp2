#include <gtest/gtest.h>

#include "xmol/crystal/LatticeVectors.h"


using ::testing::Test;
using namespace xmol::geometry;
using namespace xmol::crystal;

class lattice_vectors_Tests : public Test{
public:
};


TEST_F(lattice_vectors_Tests, construction){
  LatticeVectors latticeVectors(1,1,1,Degrees(90),Degrees(90),Degrees(90));

  EXPECT_DOUBLE_EQ(latticeVectors[0].x(),1);
  EXPECT_DOUBLE_EQ(latticeVectors[1].y(),1);
  EXPECT_DOUBLE_EQ(latticeVectors[2].z(),1);

  latticeVectors.scale_by(10);

  EXPECT_DOUBLE_EQ(latticeVectors[0].x(),10);
  EXPECT_DOUBLE_EQ(latticeVectors[1].y(),10);
  EXPECT_DOUBLE_EQ(latticeVectors[2].z(),10);

  LatticeVectors latticeVectors2(XYZ(1,0,0),XYZ(0,1,0),XYZ(0,0,1));

}


TEST_F(lattice_vectors_Tests, best_shift){
  LatticeVectors latticeVectors(XYZ(1,0,0),XYZ(0,1,0),XYZ(0,0,1));
  BestShiftFinder bestShiftFinder(latticeVectors);

  auto ref = XYZ(0,0,0);
  auto var = XYZ(1,2,3);

  XYZ shift;
  double dr;
  std::tie(dr, shift)= bestShiftFinder.find_best_shift(ref,var);

  EXPECT_DOUBLE_EQ(dr,0);
  EXPECT_DOUBLE_EQ(-1,shift.x());
  EXPECT_DOUBLE_EQ(-2,shift.y());
  EXPECT_DOUBLE_EQ(-3,shift.z());



}

TEST_F(lattice_vectors_Tests, best_shift2){

  LatticeVectors latticeVectors(XYZ(1,4,1),XYZ(5,1,1),XYZ(7,1,4));
  BestShiftFinder bestShiftFinder(latticeVectors);

  auto ref = XYZ(0,0,0);
  auto var = latticeVectors.translate(ref,6,-1,4);

  XYZ shift;
  double dr;
  std::tie(dr, shift) = bestShiftFinder.find_best_shift(ref,var);
  var = var + shift;

  EXPECT_DOUBLE_EQ(dr,0);
  EXPECT_DOUBLE_EQ(var.x(),ref.x());
  EXPECT_DOUBLE_EQ(var.y(),ref.y());
  EXPECT_DOUBLE_EQ(var.z(),ref.z());

}


TEST_F(lattice_vectors_Tests, best_shift3){

  LatticeVectors latticeVectors(XYZ(1,0,0),XYZ(0,1,0),XYZ(0,0,1));
  BestShiftFinder bestShiftFinder(latticeVectors);

  bestShiftFinder.scale_lattice_by(1.5);
  latticeVectors.scale_by(1.5);

  auto ref = XYZ(0,0,0);
  auto var = latticeVectors.translate(ref,1,2,3);

  XYZ shift;
  double dr;
  std::tie(dr, shift)= bestShiftFinder.find_best_shift(ref,var);

  EXPECT_DOUBLE_EQ(dr,0);
  EXPECT_DOUBLE_EQ(-1.5,shift.x());
  EXPECT_DOUBLE_EQ(-3.0,shift.y());
  EXPECT_DOUBLE_EQ(-4.5,shift.z());
}