#include <gtest/gtest.h>

#include "xmol/polymer/TorsionAngle.h"
#include "xmol/polymer/exceptions.h"
#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include <cmath>

using ::testing::Test;
using namespace xmol::geometry;
using namespace xmol::polymer;
using namespace xmol::pdb;

class TorsionAngleTests : public Test {
public:
};

TEST_F(TorsionAngleTests, test1) {
  Frame frame(1);
  Chain& c = frame.emplace(ChainName("A"));
  Residue& r = c.emplace(ResidueName("LYS"), residueId_t(1));
  r.emplace(AtomName("A"), atomId_t(0), XYZ(1, 0, 0));
  r.emplace(AtomName("B"), atomId_t(1), XYZ(0, 0, 0));
  r.emplace(AtomName("C"), atomId_t(2), XYZ(0, 0, 1));
  r.emplace(AtomName("D"), atomId_t(3), XYZ(0, 1, 1));

  auto t = TorsionAngle(r[AtomName("A")], r[AtomName("B")], r[AtomName("C")], r[AtomName("D")],
                        [](Atom& a, Atom& b, Atom& c, Atom& d) {
                          Atom* atoms[] = {&d};
                          return AtomSelection(std::begin(atoms), std::end(atoms));
                        });
  EXPECT_DOUBLE_EQ(t.value().degrees(), 90);

  t.set(Degrees(90)); // noop
  EXPECT_DOUBLE_EQ(t.value().degrees(), 90);

  t.set(Degrees(0));
  EXPECT_TRUE(fabs(t.value().degrees() - 0) < 1e-10);

  EXPECT_TRUE(fabs(r[AtomName("D")].r().x() - 1) < 1e-10);
  EXPECT_TRUE(fabs(r[AtomName("D")].r().y() - 0) < 1e-10);
  EXPECT_TRUE(fabs(r[AtomName("D")].r().z() - 1) < 1e-10);

  auto throws1 = [&] {
    auto t2 = TorsionAngle(r[AtomName("A")], r[AtomName("B")], r[AtomName("C")], r[AtomName("D")],
                           [](Atom& a, Atom& b, Atom& c, Atom& d) {
                             Atom* atoms[] = {&a, &d};
                             return AtomSelection(std::begin(atoms), std::end(atoms));
                           });
  };

  EXPECT_THROW(throws1(), PolymerException);
  auto throws2 = [&] {
    auto t3 = TorsionAngle(r[AtomName("A")], r[AtomName("B")], r[AtomName("C")], r[AtomName("D")],
                           [](Atom& a, Atom& b, Atom& c, Atom& d) {
                             Atom* atoms[] = {&b, &c};
                             return AtomSelection(std::begin(atoms), std::end(atoms));
                           });
  };

  EXPECT_THROW(throws2(), PolymerException);

  auto t4 = TorsionAngle(r[AtomName("A")], r[AtomName("B")], r[AtomName("C")], r[AtomName("D")]);
  EXPECT_TRUE(fabs(t4.value().degrees() - 0) < 1e-10);

  EXPECT_THROW(t4.set(Degrees(10)), PolymerException);


}
TEST_F(TorsionAngleTests, test_backbone_angles){
  auto files ={
      "pdb/rcsb/1PGB.pdb",
      "pdb/rcsb/5BMG.pdb",
      "pdb/rcsb/5BMH.pdb",
  };
  for(auto& file: files){
    auto frame = PdbFile(file).get_frame();

    for(auto&r : frame.asResidues()){

      auto phi = TorsionAngleFactory::phi(r);
      auto psi = TorsionAngleFactory::psi(r);
      auto omega = TorsionAngleFactory::omega(r);
      if (!!phi){
        phi.value().set(Degrees(180.0));
      }
      if (!!psi){
        psi.value().set(Degrees(180.0));
      }
      if (!!omega){
        omega.value().set(Degrees(180.0));
      }
    }
  }
}
