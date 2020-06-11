#include <gtest/gtest.h>

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
  }
};

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

  //  EXPECT_EQ(C.serial(),2);// todo: enable when .serial() is supported
  EXPECT_EQ(C.frame(), frame);

  {
    Frame frame2 = frame;
    EXPECT_NE(frame2, frame);
    EXPECT_EQ(frame2.n_molecules(), frame.n_molecules());

    //    const MoleculeRef C2 = frame2.molecules()[-1];
    MoleculeRef C2 = frame2.molecules()[frame2.n_molecules() - 1]; // todo: switch to negative index
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
//    EXPECT_EQ(C2.serial(), C.serial()); /// todo: enable

    ResidueRef R = C.residues()[C.residues().size() - 1];
    ResidueRef R2 = C2.residues()[C2.residues().size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R.size() - 1];
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
    //    EXPECT_EQ(C2.serial(),C.serial()); // todo: enable

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R.size() - 1];
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
    //    EXPECT_EQ(C2.serial(), C.serial()); // todo: enable

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R.size() - 1];
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
    //    EXPECT_EQ(C2.serial(), C.serial()); // todo:enable

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R.size() - 1];
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

    MoleculeRef C2 = frame2.molecules()[frame2.n_molecules() - 1];
    EXPECT_NE(C2, C);
    EXPECT_EQ(C2.name(), C.name());
    //    EXPECT_EQ(C2.serial(),C.serial());

    ResidueRef R = C.residues()[C.size() - 1];
    ResidueRef R2 = C2.residues()[C.size() - 1];
    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    R2.name(ResidueName("XXX"));
    EXPECT_NE(R2.name(), R.name());
    R2 = R;
    EXPECT_EQ(R2.molecule(), C2);

    EXPECT_NE(R2, R);
    EXPECT_EQ(R2.name(), R.name());
    EXPECT_EQ(R2.id(), R.id());
    EXPECT_EQ(R2.size(), R.size());

    AtomRef A = R.atoms()[R.size() - 1];
    AtomRef A2 = R2.atoms()[R.size() - 1];

    A2.id(99);
    EXPECT_NE(A2.id(), A.id());
    A2 = A;
    EXPECT_EQ(A2.id(), A.id());

    EXPECT_NE(A2, A);
    EXPECT_EQ(A2.name(), A.name());
    EXPECT_EQ(A2.id(), A.id());
    EXPECT_EQ(A2.r().x(), A.r().x());
    EXPECT_EQ(A2.r().y(), A.r().y());
    EXPECT_EQ(A2.r().z(), A.r().z());
  }
}

// todo: enable
// TEST_F(AtomTests, brakets){
//
//  Frame f;
//
//  MoleculeRef c = f.add_molecule();
//  ResidueRef r = c.add_residue().name("GLY").id(1);
//  AtomRef a = r.add_atom().name("CA").id(1);
//
//  { // cover const functions
//    const Frame& frame = f;
//    EXPECT_EQ(frame.molecules()[0],c);
//    EXPECT_EQ(frame.molecules()[0][1],r);
//    EXPECT_EQ(frame.molecules()[0][1][AtomName("CA")],a);
//
//    EXPECT_ANY_THROW(frame.molecules()[1]);
//    EXPECT_ANY_THROW(frame.molecules()[0][2]);
//    EXPECT_ANY_THROW(frame.molecules()[0][1][AtomName("CB")]);
//  }
//
//
//  EXPECT_EQ(f.molecules()[0],c);
//  EXPECT_EQ(f.molecules()[0][1],r);
//  EXPECT_EQ(f.molecules()[0][1][AtomName("CA")],a);
//
//  EXPECT_ANY_THROW(f.molecules()[1]);
//  EXPECT_ANY_THROW(f.molecules()[0][2]);
//  EXPECT_ANY_THROW(f.molecules()[0][1][AtomName("CB")]);
//
//  r.id(42);
//  a.name("XX");
//
//  EXPECT_EQ(f.molecules()[0][42],r);
//  EXPECT_EQ(f.molecules()[0][42][AtomName("XX")],a);
//
//  EXPECT_ANY_THROW(f.molecules()[0][2]);
//  EXPECT_ANY_THROW(f.molecules()[0][42][AtomName("YY")]);
//
//}

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
  // todo : enable
  //  EXPECT_EQ(atoms.slice_range().size(), 7*10+20*7);
  //  EXPECT_EQ(atoms.slice_range({},{},-1).size(), 7*10+20*7);
  //  EXPECT_EQ(atoms.slice_range(10,{},1).size(), 7*10+20*7-10);
  //  EXPECT_EQ(atoms.slice_range({},10).size(),10);
  //  EXPECT_EQ(atoms.slice_range({},10,2).size(),5);
  //  EXPECT_EQ(atoms.slice_range(0,10,-2).size(),0);
  //  EXPECT_EQ(atoms.slice_range(10,{},-2).size(),6);
  //  EXPECT_EQ(frame.atoms().slice(10,{},-2).size(),6);
  //  EXPECT_EQ(frame.atoms().slice({},{},-1).size(), 7*10+20*7);
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

  EXPECT_EQ(*c[1].next(), c[2]);
  EXPECT_EQ(*c[2].next(), c[3]);
  EXPECT_EQ(*c[3].next(), c[4]);
  EXPECT_FALSE(!!c[4].next());

  EXPECT_FALSE(!!c[1].prev());
  EXPECT_EQ(*c[2].prev(), c[1]);
  EXPECT_EQ(*c[3].prev(), c[2]);
  EXPECT_EQ(*c[4].prev(), c[3]);

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
