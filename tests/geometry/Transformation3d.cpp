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
