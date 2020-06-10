#include <gtest/gtest.h>

#include "xmol/ProteinTorsionAngleFactory.h"
#include "xmol/TorsionAngle.h"
#include "xmol/io/PdbInputFile.h"
#include "xmol/proxy/selections.h"
#include <cmath>

using ::testing::Test;
using namespace xmol::proxy;
using namespace xmol::geom;
using namespace xmol::io;
using namespace xmol;

class TorsionAngleTests : public Test {
public:
};

TEST_F(TorsionAngleTests, test1) {
  Frame frame;
  MoleculeRef mol = frame.add_molecule().name("A");
  ResidueRef r = mol.add_residue().name("LYS").id(1);
  smart::AtomSmartRef a = r.add_atom().name("A").id(0).r(XYZ(1, 0, 0));
  smart::AtomSmartRef b = r.add_atom().name("B").id(1).r(XYZ(0, 0, 0));
  smart::AtomSmartRef c = r.add_atom().name("C").id(2).r(XYZ(0, 0, 1));
  smart::AtomSmartRef d = r.add_atom().name("D").id(3).r(XYZ(0, 1, 1));

  auto t = TorsionAngle(a, b, c, d, [](AtomRef& a, AtomRef& b, AtomRef& c, AtomRef& d) {
    AtomRef atoms[] = {d};
    return AtomSelection(atoms);
  });
  EXPECT_DOUBLE_EQ(t.value().degrees(), 90);

  t.set(Degrees(90)); // noop
  EXPECT_DOUBLE_EQ(t.value().degrees(), 90);

  t.set(Degrees(0));
  EXPECT_TRUE(fabs(t.value().degrees() - 0) < 1e-10);

  EXPECT_TRUE(fabs(d.r().x() - 1) < 1e-10);
  EXPECT_TRUE(fabs(d.r().y() - 0) < 1e-10);
  EXPECT_TRUE(fabs(d.r().z() - 1) < 1e-10);

  auto throws1 = [&] {
    auto t2 = TorsionAngle(a, b, c, d, [](AtomRef& a, AtomRef& b, AtomRef& c, AtomRef& d) {
      AtomRef atoms[] = {a, d};
      return AtomSelection(atoms);
    });
  };

  EXPECT_THROW(throws1(), GeomError);
  auto throws2 = [&] {
    auto t3 = TorsionAngle(a, b, c, d, [](AtomRef& a, AtomRef& b, AtomRef& c, AtomRef& d) {
      AtomRef atoms[] = {b, c};
      return AtomSelection(atoms);
    });
  };

  EXPECT_THROW(throws2(), GeomError);

  auto t4 = TorsionAngle(a, b, c, d);
  EXPECT_TRUE(fabs(t4.value().degrees() - 0) < 1e-10);

  EXPECT_THROW(t4.set(Degrees(10)), GeomError);
}
TEST_F(TorsionAngleTests, test_backbone_angles) {

  auto files = {
      "pdb/rcsb/1PGB.pdb",
      "pdb/rcsb/5BMG.pdb",
      "pdb/rcsb/5BMH.pdb",
  };
  for (auto& file : files) {
    auto frame = PdbInputFile(file).frames()[0];
    for (auto&& r : frame.residues()) {

      auto phi = TorsionAngleFactory::phi(r);
      auto psi = TorsionAngleFactory::psi(r);
      auto omega = TorsionAngleFactory::omega(r);
      if (!!phi) {
        phi.value().set(Degrees(180.0));
      } else {
        //        std::cout << "no   phi for " << r.name().str() << r.id().serial << std::endl;
      }
      if (!!psi) {
        psi.value().set(Degrees(180.0));
      } else {
        //        std::cout << "no   psi for " << r.name().str() << r.id().serial << std::endl;
      }
      if (!!omega) {
        omega.value().set(Degrees(180.0));
      } else {
        //        std::cout << "no omega for " << r.name().str() << r.id().serial << std::endl;
      }
    }
  }
}
