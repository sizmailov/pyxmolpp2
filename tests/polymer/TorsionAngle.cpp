#include <gtest/gtest.h>

#include "xmol/polymer/TorsionAngle.h"
#include "xmol/polymer/exceptions.h"
#include <cmath>

using ::testing::Test;
using namespace xmol::geometry;
using namespace xmol::polymer;

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
