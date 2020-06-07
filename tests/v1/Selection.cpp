#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/selections.h"
#include "xmol/v1/proxy/smart/selections.h"
#include "xmol/v1/proxy/smart/spans.h"
#include "xmol/v1/proxy/spans-impl.h"

using ::testing::Test;
using namespace xmol::v1;
using namespace xmol::v1::proxy;
using namespace xmol::v1::proxy::smart;

class SelectionTests : public Test {

public:
  [[nodiscard]] static Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) {
    Frame frame;
    add_polyglycines(chain_sizes, frame);
    return frame;
  }

  void static add_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes, Frame& frame) {
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
  }
};

TEST_F(SelectionTests, filter) {
  auto frame = make_polyglycines({{"A", 10}, {"B", 20}});
  auto atoms = frame.atoms();
  EXPECT_EQ(atoms.size(), 30 * 7);
  proxy::AtomSelection sel(atoms);
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

TEST_F(SelectionTests, inplace_set_operations) {
  auto frame = make_polyglycines({{"A", 1}, {"B", 3}});
  auto a = frame.atoms().filter([](AtomRef& a) { return a.molecule().name() == "A"; });
  auto b = frame.atoms().filter([](AtomRef& a) { return a.molecule().name() == "B"; });
  ASSERT_EQ(a.size() + b.size(), frame.atoms().size());
  auto c = a;
  c |= a;
  ASSERT_EQ(c.size(), a.size());
  c |= b;
  ASSERT_EQ(c.size(), a.size() + b.size());
  c &= b;
  ASSERT_EQ(c.size(), b.size());
  c -= a;
  ASSERT_EQ(c.size(), b.size());
  c -= b;
  ASSERT_EQ(c.size(), 0);
}

TEST_F(SelectionTests, full_order) {
  auto frame = make_polyglycines({{"A", 1}, {"B", 3}, {"C", 5}});
  auto check_ordering = [](auto&& c) {
    using selection_t = std::remove_reference_t<decltype(c)>;
    auto less_than = typename selection_t::LessThanComparator{};
    for (int i = 0; i < c.size(); i++) {
      for (int j = 0; j < c.size(); j++) {
        ASSERT_EQ(less_than(c[i], c[j]), i < j);
      }
    }
    auto equal = typename selection_t::EqualComparator{};
    for (int i = 0; i < c.size(); i++) {
      for (int j = 0; j < c.size(); j++) {
        ASSERT_EQ(equal(c[i], c[j]), i == j);
      }
    }
  };
  check_ordering(proxy::AtomSelection(frame.atoms()));
  check_ordering(proxy::ResidueSelection(frame.residues()));
  check_ordering(proxy::MoleculeSelection(frame.molecules()));
}

TEST_F(SelectionTests, smart_atom_selection) {
  auto frame = make_polyglycines({{"A", 1}});
  AtomSelection plain_selection = AtomSelection(frame.atoms());
  AtomSmartSelection atoms = plain_selection;
  EXPECT_EQ(7, atoms.size());
  for (int i = 1; i < 10; ++i) {
    add_polyglycines({{"A", 1}}, frame);
    for (int j = 0; j < 7; j++) {
      EXPECT_EQ(atoms[j], frame.atoms()[j]);
    }
  }
  for (int j = 0; j < 7; j++) {
    EXPECT_NE(atoms[j], plain_selection[j]); // no UB, just comparing pointers
  }
}

TEST_F(SelectionTests, smart_atom_exceptions) {
  auto frame = make_polyglycines({{"A", 1}});
  AtomSelection plain_selection = AtomSelection(frame.atoms());
  AtomSmartSelection atoms = plain_selection;
  frame = {};
  ASSERT_THROW(atoms.residues(), DeadFrameAccessError);
  ASSERT_THROW(atoms.molecules(), DeadFrameAccessError);
  ASSERT_THROW(atoms.filter([](const AtomRef&) { return false; }), DeadFrameAccessError);
  ASSERT_THROW(atoms.unite(atoms), DeadFrameAccessError);
  ASSERT_THROW(atoms.intersect(atoms), DeadFrameAccessError);
  ASSERT_THROW(atoms.substract(atoms), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms.empty()), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms.size()), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms.begin()), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms.end()), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms |= atoms), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms &= atoms), DeadFrameAccessError);
  ASSERT_THROW(static_cast<void>(atoms -= atoms), DeadFrameAccessError);
}

TEST_F(SelectionTests, smart_atom_ref_count) {
  auto frame = make_polyglycines({{"A", 1}});
  auto atoms = AtomSelection(frame.atoms()).smart();
  EXPECT_EQ(frame.n_references<AtomSmartSelection>(), 1);
  atoms = atoms - atoms;
  EXPECT_EQ(frame.n_references<AtomSmartSelection>(), 0);
  atoms = AtomSelection(frame.atoms()).smart();
  EXPECT_EQ(frame.n_references<AtomSmartSelection>(), 1);
  atoms = atoms & AtomSelection{};
  EXPECT_EQ(frame.n_references<AtomSmartSelection>(), 0);
}

TEST_F(SelectionTests, smart_atom_multiple_frames) {
  auto frame = make_polyglycines({{"A", 1}});
  auto frame2 = make_polyglycines({{"A", 1}});

  EXPECT_THROW(frame.atoms() | frame2.atoms(), MultipleFramesSelectionError);
  EXPECT_THROW(frame.atoms() - frame2.atoms(), MultipleFramesSelectionError);
  EXPECT_THROW(frame.atoms() & frame2.atoms(), MultipleFramesSelectionError);
}