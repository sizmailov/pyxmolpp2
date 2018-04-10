#include <gtest/gtest.h>

#include "xmol/polymer/Atom.h"


using ::testing::Test;
using namespace xmol::polymer;



class AtomTests : public Test{
public:
};

TEST_F(AtomTests, composition){
  Frame frame(0);

  frame.emplace(ChainName("A"));
  frame.emplace(ChainName("B"));
  Chain& C = frame.emplace(ChainName("C"));

  C.emplace(ResidueName("GLY"),1);
  Residue& C_GLY2 = C.emplace(ResidueName("GLY"),2);

  C_GLY2.emplace(AtomName("CA"),1,XYZ{});
  C_GLY2.emplace(AtomName("CB"),2,XYZ{});
  Atom& C_GLY2_CG = C_GLY2.emplace(AtomName("CG"),3,XYZ{1,2,3});

  EXPECT_EQ(3,frame.size());
  EXPECT_EQ(2,C.size());
  EXPECT_EQ(3,C_GLY2.size());

  EXPECT_EQ(C_GLY2_CG.r().x,1);
  EXPECT_EQ(C_GLY2_CG.r().y,2);
  EXPECT_EQ(C_GLY2_CG.r().z,3);


  EXPECT_EQ(C_GLY2_CG.name(),AtomName("CG"));
  EXPECT_EQ(C_GLY2_CG.name().str(),"CG");

  EXPECT_EQ(C_GLY2_CG.index(),2);
  EXPECT_EQ(C_GLY2_CG.id(),3);


  C_GLY2_CG.set_name(AtomName("CGG"));

  C_GLY2_CG.set_id(4);

  C_GLY2_CG.set_r(XYZ{4,5,6});

  EXPECT_EQ(C_GLY2_CG.r().x,4);
  EXPECT_EQ(C_GLY2_CG.r().y,5);
  EXPECT_EQ(C_GLY2_CG.r().z,6);

  EXPECT_EQ(C_GLY2_CG.name(),AtomName("CGG"));
  EXPECT_EQ(C_GLY2_CG.name().str(),"CGG");

  EXPECT_EQ(C_GLY2_CG.index(),2);
  EXPECT_EQ(C_GLY2_CG.id(),4);

  EXPECT_EQ(C_GLY2_CG.residue(),C_GLY2);
  EXPECT_EQ(C_GLY2.size(),3);
  EXPECT_EQ(C_GLY2.all()[-1],C_GLY2_CG);
  EXPECT_EQ(C_GLY2.all()[2],C_GLY2_CG);


  EXPECT_EQ(C_GLY2.name(),ResidueName("GLY"));
  EXPECT_EQ(C_GLY2.name().str(),"GLY");
  C_GLY2.set_name(ResidueName("LYS"));
  EXPECT_EQ(C_GLY2.name(),ResidueName("LYS"));
  EXPECT_EQ(C_GLY2.name().str(),"LYS");

  EXPECT_EQ(C_GLY2.id(),2);
  C_GLY2.set_id(2);
  EXPECT_EQ(C_GLY2.id(),2);

  EXPECT_EQ(C_GLY2.index(),1);

  EXPECT_EQ(C_GLY2.chain(),C);

  EXPECT_EQ(C.name(),ChainName("C"));
  C.set_name(ChainName("X"));
  EXPECT_EQ(C.name(),ChainName("X"));

  EXPECT_EQ(C.index(),2);
  EXPECT_EQ(C.frame(),frame);



  {
    Frame frame2 = frame;
    EXPECT_NE(frame2,frame);
    EXPECT_EQ(frame2.size(),frame.size());

    const Chain& C2 = frame2.all()[-1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(),C.name());
    EXPECT_EQ(C2.index(),C.index());

    const Residue& R = C.all()[-1];
    const Residue& R2 = C2.all()[-1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());


    const Atom& A = R.all()[-1];
    const Atom& A2 = R2.all()[-1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(),A.name());
    EXPECT_EQ(A2.index(),A.index());
    EXPECT_EQ(A2.id(),A.id());
    EXPECT_EQ(A2.r().x,A.r().x);
    EXPECT_EQ(A2.r().y,A.r().y);
    EXPECT_EQ(A2.r().z,A.r().z);
  }
  {
    Frame tmp = frame;
    Frame frame2 = std::move(tmp);
    EXPECT_NE(frame2,frame);
    EXPECT_EQ(frame2.size(),frame.size());

    const Chain& C2 = frame2.all()[-1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(),C.name());
    EXPECT_EQ(C2.index(),C.index());

    const Residue& R = C.all()[-1];
    const Residue& R2 = C2.all()[-1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());


    const Atom& A = R.all()[-1];
    const Atom& A2 = R2.all()[-1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(),A.name());
    EXPECT_EQ(A2.index(),A.index());
    EXPECT_EQ(A2.id(),A.id());
    EXPECT_EQ(A2.r().x,A.r().x);
    EXPECT_EQ(A2.r().y,A.r().y);
    EXPECT_EQ(A2.r().z,A.r().z);
  }

  {
    Frame tmp = frame;
    Frame frame2(0);
    frame2 = std::move(tmp);
    EXPECT_NE(frame2,frame);
    EXPECT_EQ(frame2.size(),frame.size());

    const Chain& C2 = frame2.all()[-1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(),C.name());
    EXPECT_EQ(C2.index(),C.index());

    const Residue& R = C.all()[-1];
    const Residue& R2 = C2.all()[-1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());


    const Atom& A = R.all()[-1];
    const Atom& A2 = R2.all()[-1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(),A.name());
    EXPECT_EQ(A2.index(),A.index());
    EXPECT_EQ(A2.id(),A.id());
    EXPECT_EQ(A2.r().x,A.r().x);
    EXPECT_EQ(A2.r().y,A.r().y);
    EXPECT_EQ(A2.r().z,A.r().z);
  }

  {
    Frame frame2(0);
    frame2 = frame;
    EXPECT_NE(frame2,frame);
    EXPECT_EQ(frame2.size(),frame.size());

    const Chain& C2 = frame2.all()[-1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(),C.name());
    EXPECT_EQ(C2.index(),C.index());

    const Residue& R = C.all()[-1];
    const Residue& R2 = C2.all()[-1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());


    const Atom& A = R.all()[-1];
    const Atom& A2 = R2.all()[-1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(),A.name());
    EXPECT_EQ(A2.index(),A.index());
    EXPECT_EQ(A2.id(),A.id());
    EXPECT_EQ(A2.r().x,A.r().x);
    EXPECT_EQ(A2.r().y,A.r().y);
    EXPECT_EQ(A2.r().z,A.r().z);
  }


  {
    Frame frame2(0);
    frame2 = frame;
    EXPECT_NE(frame2,frame);
    EXPECT_EQ(frame2.size(),frame.size());

    Chain& C2 = frame2.all()[-1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(),C.name());
    EXPECT_EQ(C2.index(),C.index());

    Residue& R = C.all()[-1];
    Residue& R2 = C2.all()[-1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());

    R2.set_name(ResidueName("XXX"));
    EXPECT_NE(R2.name(),R.name());
    R2 = R;
    EXPECT_EQ(R2.chain(),C2);

    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(),R.name());
    EXPECT_EQ(R2.index(),R.index());
    EXPECT_EQ(R2.id(),R.id());
    EXPECT_EQ(R2.size(),R.size());

    Atom& A = R.all()[-1];
    Atom& A2 = R2.all()[-1];

    A2.set_id(99);
    EXPECT_NE(A2.id(),A.id());
    A2 = A;
    EXPECT_EQ(A2.id(),A.id());

    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(),A.name());
    EXPECT_EQ(A2.index(),A.index());
    EXPECT_EQ(A2.id(),A.id());
    EXPECT_EQ(A2.r().x,A.r().x);
    EXPECT_EQ(A2.r().y,A.r().y);
    EXPECT_EQ(A2.r().z,A.r().z);
  }
}

TEST_F(AtomTests, brakets){
  Frame f(1);

  Chain&c = f.emplace(ChainName(""));
  Residue&r = c.emplace(ResidueName("GLY"),1);
  Atom&a = r.emplace(AtomName("CA"),1,XYZ{0,0,0});

  { // cover const functions
    const Frame& frame = f;
    EXPECT_EQ(frame[0],c);
    EXPECT_EQ(frame[0][1],r);
    EXPECT_EQ(frame[0][1][AtomName("CA")],a);

    EXPECT_ANY_THROW(frame[1]);
    EXPECT_ANY_THROW(frame[0][2]);
    EXPECT_ANY_THROW(frame[0][1][AtomName("CB")]);
  }

  EXPECT_EQ(f[0],c);
  EXPECT_EQ(f[0][1],r);
  EXPECT_EQ(f[0][1][AtomName("CA")],a);

  EXPECT_ANY_THROW(f[1]);
  EXPECT_ANY_THROW(f[0][2]);
  EXPECT_ANY_THROW(f[0][1][AtomName("CB")]);

  r.set_id(42);
  a.set_name(AtomName("XX"));

  EXPECT_EQ(f[0][1],r);
  EXPECT_EQ(f[0][1][AtomName("XX")],a);

  EXPECT_ANY_THROW(f[0][2]);
  EXPECT_ANY_THROW(f[0][42][AtomName("YY")]);

}

