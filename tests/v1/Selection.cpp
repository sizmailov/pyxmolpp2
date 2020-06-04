#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"

using ::testing::Test;
using namespace xmol::v1;

class SelectionTests : public Test {

public:
  [[nodiscard]] static Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) {
    Frame frame;
    int aid = 1;
    int rid = 1;
    for (auto& chN_S : chain_sizes) {
      auto c = frame.add_molecule(MoleculeName(chN_S.first));
      for (int i = 0; i < chN_S.second; i++) {
        auto r = c.add_residue(ResidueName("GLY"), ResidueId(rid++));
        r.add_atom(AtomName("N"), AtomId(aid++));
        r.add_atom(AtomName("H"), AtomId(aid++));
        r.add_atom(AtomName("CA"), AtomId(aid++));
        r.add_atom(AtomName("HA2"), AtomId(aid++));
        r.add_atom(AtomName("HA3"), AtomId(aid++));
        r.add_atom(AtomName("C"), AtomId(aid++));
        r.add_atom(AtomName("O"), AtomId(aid++));
      }
    }
    return frame;
  }
};

TEST_F(SelectionTests, filter) {
  auto frame = make_polyglycines({{"A", 10}, {"B", 20}});
  auto atoms = frame.atoms();
  EXPECT_EQ(atoms.size(), 30 * 7);
  Selection<proxy::AtomRef> sel(atoms);
  EXPECT_EQ(sel.size(), atoms.size());
  auto ca = sel.filter([](proxy::AtomRef& atom) { return atom.name() == "CA"; });
  EXPECT_EQ(ca.size(), 30);
  auto a_ca = ca.filter([](proxy::AtomRef& atom) { return atom.molecule().name() == "A"; });
  auto b_ca = ca.filter([](proxy::AtomRef& atom) { return atom.molecule().name() == "B"; });
  EXPECT_EQ(a_ca.size(), 10);
  EXPECT_EQ(b_ca.size(), 20);
  auto a_r1_ca = a_ca.filter([](proxy::AtomRef& atom) { return atom.residue().id() == 1; });
  auto b_r1_ca = b_ca.filter([](proxy::AtomRef& atom) { return atom.residue().id() == 1; });
  EXPECT_EQ(a_r1_ca.size(), 1);
  EXPECT_EQ(b_r1_ca.size(), 0);
  
}