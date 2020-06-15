#include <gtest/gtest.h>
#include <xmol/proxy/smart/spans.h>

#include "test_common.h"
#include "xmol/Frame.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::test;
using namespace xmol::proxy;

class AtomTests : public Test {
public:
  Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) const {
    Frame frame;
    add_polyglycines(chain_sizes, frame);
    return frame;
  }
};

TEST_F(AtomTests, add_molecules) {
  Frame frame;
  int n = 150;
  for (int i = 0; i < n; ++i) {
    auto mol = frame.add_molecule();
    ASSERT_EQ(mol.frame(), frame);
    ASSERT_EQ(frame.n_molecules(), frame.molecules().size());
  }
}

TEST_F(AtomTests, add_residues) {
  Frame frame;
  auto mol = frame.add_molecule();
  int n = 150;
  for (int i = 0; i < n; ++i) {
    auto res = mol.add_residue();
    ASSERT_EQ(res.frame(), frame);
    ASSERT_EQ(res.molecule(), mol);
    ASSERT_EQ(frame.n_residues(), frame.residues().size());
  }
}

TEST_F(AtomTests, add_residues_2) {
  Frame frame;
  auto mol1 = frame.add_molecule();
  mol1.add_residue();
  frame.add_molecule();
}

TEST_F(AtomTests, add_atoms) {
  Frame frame;
  auto mol = frame.add_molecule();
  auto res = mol.add_residue();
  int n = 150;
  for (int i = 0; i < n; ++i) {
    auto atom = res.add_atom();
    ASSERT_EQ(atom.frame(), frame);
    ASSERT_EQ(atom.residue(), res);
    ASSERT_EQ(atom.molecule(), mol);
    ASSERT_EQ(frame.n_molecules(), frame.molecules().size());
    ASSERT_EQ(frame.n_residues(), frame.residues().size());
    ASSERT_EQ(frame.n_atoms(), frame.atoms().size());
  }
}

TEST_F(AtomTests, add_atoms_2) {
  Frame frame;
  int n = 150;
  for (int i = 0; i < n; ++i) {
    auto last_mol = frame.add_molecule();
    auto last_res = last_mol.add_residue();
    auto last_atom = last_res.add_atom();
    ASSERT_EQ(last_atom.frame(), frame);
    ASSERT_EQ(last_atom.residue(), last_res);
    ASSERT_EQ(last_atom.molecule(), last_mol);
    ASSERT_EQ(frame.n_molecules(), frame.molecules().size());
    ASSERT_EQ(frame.n_residues(), frame.residues().size());
    ASSERT_EQ(frame.n_atoms(), frame.atoms().size());
    ASSERT_EQ(frame.n_molecules(), i + 1);
    ASSERT_EQ(frame.n_residues(), i + 1);
    ASSERT_EQ(frame.n_atoms(), i + 1);
    for (auto mol : frame.molecules()) {
      ASSERT_EQ(mol.size(), mol.residues().size()) << "Molecule " << i;
      for (auto res : mol.residues()) {
        ASSERT_EQ(res.size(), res.atoms().size()) << "Residue " << i;
        ASSERT_EQ(res.molecule(), mol) << "Residue " << i;
        for (auto atom : res.atoms()) {
          ASSERT_EQ(atom.residue(), res) << "Atom " << i;
        }
      }
    }
  }
  auto atoms = frame.atoms().smart();
  for (int i = 0; i < atoms.size(); i++) {
    ASSERT_EQ(atoms[i].index(), i);
  }
  auto residues = frame.residues().smart();
  for (int i = 0; i < atoms.size(); i++) {
    ASSERT_EQ(residues[i].index(), i);
  }
  auto molecules = frame.molecules().smart();
  for (int i = 0; i < molecules.size(); i++) {
    ASSERT_EQ(molecules[i].index(), i);
  }
}

TEST_F(AtomTests, composition) {
  Frame frame;

  frame.add_molecule().name("A");
  frame.add_molecule().name("B");
  MoleculeRef C = frame.add_molecule().name("C");

  C.add_residue().name("GLY").id(1);
  ResidueRef C_GLY2 = C.add_residue().name("GLY").id(2);

  C_GLY2.add_atom().name("CA").id(1);
  C_GLY2.add_atom().name("CB").id(2);
  AtomRef C_GLY2_CG = C_GLY2.add_atom().name("CG").id(3).r({1, 2, 3});

  EXPECT_EQ(3, frame.n_molecules());
  EXPECT_EQ(2, C.size());
  EXPECT_EQ(3, C_GLY2.size());

  EXPECT_EQ(C_GLY2_CG.r().x(), 1);
  EXPECT_EQ(C_GLY2_CG.r().y(), 2);
  EXPECT_EQ(C_GLY2_CG.r().z(), 3);

  EXPECT_EQ(C_GLY2_CG.name(), AtomName("CG"));
  EXPECT_EQ(C_GLY2_CG.name().str(), "CG");

  EXPECT_EQ(C_GLY2_CG.id(), 3);

  C_GLY2_CG.name("CGG");

  C_GLY2_CG.id(4);

  C_GLY2_CG.r({4, 5, 6});

  EXPECT_EQ(C_GLY2_CG.r().x(), 4);
  EXPECT_EQ(C_GLY2_CG.r().y(), 5);
  EXPECT_EQ(C_GLY2_CG.r().z(), 6);

  EXPECT_EQ(C_GLY2_CG.name(), AtomName("CGG"));
  EXPECT_EQ(C_GLY2_CG.name().str(), "CGG");

  EXPECT_EQ(C_GLY2_CG.id(), 4);

  EXPECT_EQ(C_GLY2_CG.residue(), C_GLY2);
  EXPECT_EQ(C_GLY2.size(), 3);
  EXPECT_EQ(C_GLY2.atoms()[C_GLY2.atoms().size() - 1], C_GLY2_CG); // todo: switch to negative index
  EXPECT_EQ(C_GLY2.atoms()[2], C_GLY2_CG);

  EXPECT_EQ(C_GLY2.name(), ResidueName("GLY"));
  EXPECT_EQ(C_GLY2.name().str(), "GLY");
  C_GLY2.name(ResidueName("LYS"));
  EXPECT_EQ(C_GLY2.name(), ResidueName("LYS"));
  EXPECT_EQ(C_GLY2.name().str(), "LYS");

  EXPECT_EQ(C_GLY2.id(), 2);
  C_GLY2.id(2);
  EXPECT_EQ(C_GLY2.id(), 2);

  EXPECT_EQ(C_GLY2.molecule(), C);

  EXPECT_EQ(C.name(), MoleculeName("C"));
  C.name(MoleculeName("X"));
  EXPECT_EQ(C.name(), MoleculeName("X"));

  EXPECT_EQ(C.index(), 2);
  EXPECT_EQ(C.frame(), frame);

  {
    Frame frame2 = frame;

    EXPECT_NE(frame2, frame);
    EXPECT_EQ(frame2.n_molecules(), frame.n_molecules());

    //    const MoleculeRef C2 = frame2.molecules()[-1];
    MoleculeRef C2 = frame2.molecules()[frame2.n_molecules() - 1]; // todo: switch to negative index
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
    EXPECT_EQ(C2.index(), C.index());

    ResidueRef R = C.residues()[C.residues().size() - 1];
    ResidueRef R2 = C2.residues()[C2.residues().size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R2.size() - 1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(), A.name());
    EXPECT_EQ(A2.id(), A.id());
    EXPECT_EQ(A2.r().x(), A.r().x());
    EXPECT_EQ(A2.r().y(), A.r().y());
    EXPECT_EQ(A2.r().z(), A.r().z());
  }
  {
    Frame tmp = frame;
    Frame frame2 = std::move(tmp);
    EXPECT_NE(frame2, frame);
    EXPECT_EQ(frame2.n_molecules(), frame.n_molecules());

    MoleculeRef C2 = frame2.molecules()[frame2.n_molecules() - 1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
    EXPECT_EQ(C2.index(), C.index());

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C2.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    auto ats = R2.atoms();
    auto&& ats2 = R2.atoms();
    EXPECT_EQ(ats.size(), R2.size());
    EXPECT_EQ(ats2.size(), R2.size());
    AtomRef A2 = R2.atoms()[R2.size() - 1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(), A.name());
    EXPECT_EQ(A2.id(), A.id());
    EXPECT_EQ(A2.r().x(), A.r().x());
    EXPECT_EQ(A2.r().y(), A.r().y());
    EXPECT_EQ(A2.r().z(), A.r().z());
  }

  {
    Frame tmp = frame;
    Frame frame2;
    frame2 = std::move(tmp);
    EXPECT_NE(frame2, frame);
    EXPECT_EQ(frame2.n_molecules(), frame.n_molecules());

    MoleculeRef C2 = frame2.molecules()[frame2.n_molecules() - 1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
    EXPECT_EQ(C2.index(), C.index());

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C2.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R2.size() - 1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(), A.name());
    EXPECT_EQ(A2.id(), A.id());
    EXPECT_EQ(A2.r().x(), A.r().x());
    EXPECT_EQ(A2.r().y(), A.r().y());
    EXPECT_EQ(A2.r().z(), A.r().z());
  }

  {
    Frame frame2;
    frame2 = frame;
    EXPECT_NE(frame2, frame);
    EXPECT_EQ(frame2.n_molecules(), frame.n_molecules());

    MoleculeRef C2 = frame2.molecules()[frame.n_molecules() - 1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
    EXPECT_EQ(C2.index(), C.index());

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C2.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R2.size() - 1];
    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(), A.name());
    EXPECT_EQ(A2.id(), A.id());
    EXPECT_EQ(A2.r().x(), A.r().x());
    EXPECT_EQ(A2.r().y(), A.r().y());
    EXPECT_EQ(A2.r().z(), A.r().z());
  }
}

TEST_F(AtomTests, brakets) {

  Frame f;

  MoleculeRef c = f.add_molecule();
  ResidueRef r = c.add_residue().name("GLY").id(1);
  AtomRef a = r.add_atom().name("CA").id(1);

  { // cover const functions
    Frame& frame = f;
    EXPECT_EQ(frame.molecules()[0][1].value(), r);
    EXPECT_EQ(frame.molecules()[0][1].value()["CA"], a);

    EXPECT_ANY_THROW(frame.molecules()[0][2].value());
    EXPECT_ANY_THROW(frame.molecules()[0][1].value()["CB"].value());
  }

  EXPECT_EQ(f.molecules()[0][1].value(), r);
  EXPECT_EQ(f.molecules()[0][1].value()["CA"].value(), a);

  EXPECT_ANY_THROW(f.molecules()[0][2].value());
  EXPECT_ANY_THROW(f.molecules()[0][1].value()["CB"].value());

  r.id(42);
  a.name("XX");

  EXPECT_EQ(f.molecules()[0][42].value(), r);
  EXPECT_EQ(f.molecules()[0][42].value()["XX"].value(), a);

  EXPECT_ANY_THROW(f.molecules()[0][2].value());
  EXPECT_ANY_THROW(f.molecules()[0][42].value()["YY"].value());
}

TEST_F(AtomTests, dedicated_selections) {
  Frame f;

  MoleculeRef c = f.add_molecule();
  ResidueRef r = c.add_residue().name("GLY").id(1);
  AtomRef a = r.add_atom().name("CA").id(1);
  static_cast<void>(a);

  EXPECT_EQ(r.atoms().residues().size(), 1);
  EXPECT_EQ(r.atoms().residues().size(), 1);
  EXPECT_EQ(c.atoms().size(), 1);
  EXPECT_EQ(c.residues().atoms().residues().molecules().size(), 1);
}

TEST_F(AtomTests, selection_slices) {
  Frame frame = make_polyglycines({{"A", 10}, {"B", 20}});
  auto atoms = frame.atoms();
  EXPECT_EQ(atoms.size(), 7 * 10 + 20 * 7);

  EXPECT_EQ(atoms.slice({}, {}, {}).size(), 7 * 10 + 20 * 7);
//  EXPECT_EQ(atoms.slice({}, {}, -1).size(), 7 * 10 + 20 * 7); // todo: negative indices in slices
  EXPECT_EQ(atoms.slice(10, {}, 1).size(), 7 * 10 + 20 * 7 - 10);
  EXPECT_EQ(atoms.slice({}, 10).size(), 10);
  EXPECT_EQ(atoms.slice({}, 10, 2).size(), 5);
//  EXPECT_EQ(atoms.slice(0, 10, -2).size(), 0); // todo: negative indices in slices
//  EXPECT_EQ(atoms.slice(10, {}, -2).size(), 6); // todo: negative indices in slices
//  EXPECT_EQ(frame.atoms().slice(10, {}, -2).size(), 6); // todo: negative indices in slices
//  EXPECT_EQ(frame.atoms().slice({}, {}, -1).size(), 7 * 10 + 20 * 7); // todo: negative indices in slices
}

// todo: enable when .erase() is implemented
// TEST_F(AtomTests, deletion_invalidates_selections_1){
//  Frame frame = make_polyglycines({{"A",10},{"B",20}});
//
//  auto atoms = frame.atoms();
//  auto atoms_to_delete = frame.atoms().filter([](const AtomRef& a) { return a.id() % 2 == 0; });
//  atoms_to_delete.for_each([](AtomRef a) { a.set_deleted(); });
//  EXPECT_ANY_THROW(for (auto&a: atoms){static_cast<void>(a);});
//  EXPECT_NO_THROW(for (auto&a: atoms-atoms_to_delete){static_cast<void>(a);});
//}

// todo: enable when .erase() is implemented
// TEST_F(AtomTests, deletion_invalidates_selections_2) {
//  Frame frame = make_polyglycines({{"A", 10}, {"B", 20}});
//
//  auto atoms = frame.atoms();
//  auto residues_to_delete = frame.residues().filter([](ResidueRef r) { return r.id().m_serial % 2 == 0; });
//  auto atoms_to_delete = residues_to_delete.atoms();
//  residues_to_delete.for_each([](ResidueRef r) { r.set_deleted(); });
//  EXPECT_ANY_THROW(for (auto& a : atoms) { static_cast<void>(a); });
//  EXPECT_NO_THROW(for (auto& a : atoms - atoms_to_delete) { static_cast<void>(a); });
//}

// todo: add when .erase() implemented
TEST_F(AtomTests, access_after_resize) {
  Frame frame = make_polyglycines({{"A", 1}});

  auto chain_a = frame.atoms()[0].smart();
  auto res = chain_a.residue().smart();
  static_cast<void>(res);
  chain_a.residue().add_atom();
  auto chain_b = frame.atoms()[0].smart();

  EXPECT_EQ(chain_a, chain_b);
  EXPECT_EQ((AtomRef)chain_a, (AtomRef)chain_b);
}

TEST_F(AtomTests, refcount_2) {
  Frame frame;
  auto c = frame.add_molecule().name("A").smart();
  for (int i = 0; i < 5; i++)
    c = frame.add_molecule();
}

TEST_F(AtomTests, residue_prev_next) {
  Frame frame;
  MoleculeRef mol = frame.add_molecule().name("A");
  mol.add_residue().name("LYS").id(1);
  mol.add_residue().name("GLY").id(2);
  mol.add_residue().name("CYS").id(3);
  mol.add_residue().name("ABC").id(4);

  auto c = mol.residues();

  EXPECT_EQ(c.size(), 4);

  EXPECT_EQ(*c[0].next(), c[1]);
  EXPECT_EQ(*c[1].next(), c[2]);
  EXPECT_EQ(*c[2].next(), c[3]);
  EXPECT_FALSE(!!c[3].next());

  EXPECT_FALSE(!!c[0].prev());
  EXPECT_EQ(*c[1].prev(), c[0]);
  EXPECT_EQ(*c[2].prev(), c[1]);
  EXPECT_EQ(*c[3].prev(), c[2]);

  // todo: enable when .erase() is implemented
  //
  //  c[3].set_deleted();
  //
  //  EXPECT_EQ(c[1].next(), &c[2]);
  //  EXPECT_EQ(c[2].next(), nullptr);
  //  EXPECT_EQ(c[4].next(), nullptr);
  //
  //  EXPECT_EQ(c[1].prev(), nullptr);
  //  EXPECT_EQ(c[2].prev(), &c[1]);
  //  EXPECT_EQ(c[4].prev(), nullptr);
}
