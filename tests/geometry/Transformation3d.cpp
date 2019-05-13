#include <gtest/gtest.h>

#include "xmol/geometry/Transformation3d.h"


using ::testing::Test;
using namespace xmol::geometry;

class Transformation3dTests : public Test{
public:
};


TEST_F(Transformation3dTests,inverse){

  XYZ r{1,8,4};
  UniformScale3d U(5.0);
  Translation3d T(XYZ(1,3,7));
  Rotation3d R(XYZ(1,1,1),Degrees(97));

  R.transform(r);

  { auto G(T*R*U); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(U); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(T); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(R); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { EXPECT_LE((Transformation3d(R,T).transform(r)-(T*R).transform(r)).len(),1e-9);}
  { EXPECT_LE((Transformation3d(U).transform(r)-(U).transform(r)).len(),1e-9);}
  { Transformation3d G(R,T); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(U*T*R); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(U*R*T); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(R*T*U); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(R*U*T); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(T*R*U); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}
  { auto G(T*U*R); EXPECT_LE(((G*G.inverted()).transform(r)-r).len(),1e-9);}

}

TEST_F(Transformation3dTests, rotation_angle){
  auto m = Eigen::Matrix3d();
  m << 1.000000000000000444e+00,
      1.526556658859591229e-16,
      6.938893903907233000e-18,
      1.526556658859591229e-16,
      1.000000000000000666e+00,
      -5.551115123125786400e-17,
      0.000000000000000000e+00,
      -5.551115123125786400e-17,
      1.000000000000000444e+00;
  auto r = Rotation3d(m);

  std::cout << m << std::endl;
  std::cout << r.theta().degrees() << std::endl;

  ASSERT_DOUBLE_EQ(r.theta().degrees(),0.0);
}