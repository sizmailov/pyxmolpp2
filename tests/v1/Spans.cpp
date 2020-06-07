#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"
#include "xmol/v1/proxy/smart/spans.h"

using ::testing::Test;
using namespace xmol::v1;
using namespace xmol::v1::proxy;
using namespace xmol::v1::proxy::smart;

class SpanTests : public Test {

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

TEST_F(SpanTests, ref_count) {
  auto frame = make_polyglycines({{"A", 10}, {"B", 20}});

  auto n_mol_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };
  auto n_atom_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };
  auto n_res_refs = [&] { return frame.n_references<MoleculeSmartSpan>(); };

  ASSERT_EQ(n_mol_refs(), 0);
  ASSERT_EQ(n_res_refs(), 0);
  ASSERT_EQ(n_atom_refs(), 0);
  {
    auto atoms = frame.atoms().smart();
    auto residues = frame.residues().smart();
    auto molecules = frame.molecules().smart();

    ASSERT_EQ(n_mol_refs(), 1);
    ASSERT_EQ(n_res_refs(), 1);
    ASSERT_EQ(n_atom_refs(), 1);
    {
      auto atoms2 = atoms;
      auto residues2 = residues;
      auto molecules2 = molecules;

      ASSERT_EQ(n_mol_refs(), 2);
      ASSERT_EQ(n_res_refs(), 2);
      ASSERT_EQ(n_atom_refs(), 2);

      {
        auto atoms3 = molecules.atoms().smart();
        auto residues3 = molecules.residues().smart();
        auto molecule3 = residues.molecules().smart();

        ASSERT_EQ(n_mol_refs(), 3);
        ASSERT_EQ(n_res_refs(), 3);
        ASSERT_EQ(n_atom_refs(), 3);
        {

          auto atoms4 = residues.atoms().smart();
          auto residues4 = atoms.residues().smart();
          auto molecule4 = atoms.molecules().smart();

          ASSERT_EQ(n_mol_refs(), 4);
          ASSERT_EQ(n_res_refs(), 4);
          ASSERT_EQ(n_atom_refs(), 4);
        }
        ASSERT_EQ(n_mol_refs(), 3);
        ASSERT_EQ(n_res_refs(), 3);
        ASSERT_EQ(n_atom_refs(), 3);
      }
      ASSERT_EQ(n_mol_refs(), 2);
      ASSERT_EQ(n_res_refs(), 2);
      ASSERT_EQ(n_atom_refs(), 2);
    }
    ASSERT_EQ(n_mol_refs(), 1);
    ASSERT_EQ(n_res_refs(), 1);
    ASSERT_EQ(n_atom_refs(), 1);
  }
  ASSERT_EQ(n_mol_refs(), 0);
  ASSERT_EQ(n_res_refs(), 0);
  ASSERT_EQ(n_atom_refs(), 0);
}

TEST_F(SpanTests, split_exceptinos) {
  auto frame = make_polyglycines({{"A", 10}});
  auto atoms = frame.atoms().smart();
  auto residues = frame.residues().smart();
  auto molecules = frame.molecules().smart();

  residues[residues.size()-1].add_atom({},{});
  molecules[molecules.size()-1].add_residue({},{});
  frame.add_molecule({});
  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  residues[0].add_atom({},{});
  EXPECT_THROW(static_cast<void>(atoms.size()), SpanSplitError);
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  atoms = frame.atoms(); // restore to check that residues creation doesn't affect atoms

  molecules[0].add_residue({},{});
  EXPECT_THROW(static_cast<void>(residues.size()), SpanSplitError);
  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

  residues = frame.residues();

  EXPECT_NO_THROW(static_cast<void>(atoms.size()));
  EXPECT_NO_THROW(static_cast<void>(residues.size()));
  EXPECT_NO_THROW(static_cast<void>(molecules.size()));

}