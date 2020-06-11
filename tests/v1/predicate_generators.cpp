#include <gtest/gtest.h>

#include "test_common.h"
#include "xmol/predicates/predicate_generators.h"
#include "xmol/predicates/predicates.h"
#include "xmol/proxy/selections.h"
#include "xmol/proxy/spans-impl.h"
#include "xmol/Frame.h"

using ::testing::Test;
using namespace xmol::predicates;
using namespace xmol::test;
using namespace xmol;

class PredicateGeneratorsTests : public Test {
public:
  Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) const {
    Frame frame;
    add_polyglycines(chain_sizes, frame);
    return frame;
  }
};



TEST_F(PredicateGeneratorsTests, test_atom_name_predicate){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto atoms = frame.atoms();

  {
    EXPECT_EQ(atoms.filter(aName=="CA").size(),30);
  }
  {
    EXPECT_EQ(atoms.filter(aName!="CA").size(),30*6);
  }
  {
    EXPECT_EQ(atoms.filter(aName==AtomName("CA")).size(),30);
  }
  {
    EXPECT_EQ(atoms.filter(aName!=AtomName("CA")).size(),30*6);
  }
  {
    EXPECT_EQ(atoms.filter(aName==std::string("CA")).size(),30);
  }
  {
    EXPECT_EQ(atoms.filter(aName!=std::string("CA")).size(),30*6);
  }
  {
    auto pred = aName.is_in(std::set<const char*>{"CA","CB"});
    EXPECT_EQ(atoms.filter(pred).size(),30);
  }
  {
    auto pred = aName.is_in(std::set<std::string>{"CA","C","O"});
    EXPECT_EQ(atoms.filter(pred).size(),30*3);
  }
  {
    auto pred = aName.is_in(std::set<AtomName>{AtomName{"CA"},AtomName{"C"}});
    EXPECT_EQ(atoms.filter(pred).size(),30*2);
  }
}


TEST_F(PredicateGeneratorsTests, test_residue_name_predicate){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto residues = frame.residues();

  {
    EXPECT_EQ(residues.filter(rName=="GLY").size(),30);
  }
  {
    EXPECT_EQ(residues.filter(rName!="GLY").size(),0);
  }
  {
    EXPECT_EQ(residues.filter(rName==ResidueName("GLY")).size(),30);
  }
  {
    EXPECT_EQ(residues.filter(rName!=ResidueName("GLY")).size(),0);
  }
  {
    EXPECT_EQ(residues.filter(rName==std::string("GLY")).size(),30);
  }
  {
    EXPECT_EQ(residues.filter(rName!=std::string("GLY")).size(),0);
  }
  {
    auto pred = rName.is_in(std::set<const char*>{"GLY"});
    EXPECT_EQ(residues.filter(pred).size(),30);
  }
  {
    auto pred = rName.is_in(std::set<std::string>{"LYS"});
    EXPECT_EQ(residues.filter(pred).size(),0);
  }
  {
    auto pred = rName.is_in(std::set<ResidueName>{ResidueName{"LYS"},ResidueName{"GLY"}});
    EXPECT_EQ(residues.filter(pred).size(),30);
  }
}


TEST_F(PredicateGeneratorsTests, test_chain_name_predicate){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto chains = frame.molecules();

  {
    EXPECT_EQ(chains.filter(mName=="A").size(),1);
  }
  {
    EXPECT_EQ(chains.filter(mName!="A").size(),1);
  }
  {
    EXPECT_EQ(chains.filter(mName==MoleculeName("B")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(mName!=MoleculeName("B")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(mName==std::string("A")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(mName!=std::string("B")).size(),1);
  }
  {
    auto pred = mName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(chains.filter(pred).size(),1);
  }
  {
    auto pred = mName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(chains.filter(pred).size(),2);
  }
  {
    auto pred = mName.is_in(std::set<MoleculeName>{MoleculeName{"A"}});
    EXPECT_EQ(chains.filter(pred).size(),1);
  }

}

TEST_F(PredicateGeneratorsTests, test_chain_name_predicate_on_atoms_and_residues){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto atoms = frame.atoms();

  auto residues = frame.residues();

  {
    EXPECT_EQ(atoms.filter(mName=="A").size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(mName!="A").size(),20*7);
  }
  {
    EXPECT_EQ(atoms.filter(mName==MoleculeName("B")).size(),20*7);
  }
  {
    EXPECT_EQ(atoms.filter(mName!=MoleculeName("B")).size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(mName==std::string("A")).size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(mName!=std::string("B")).size(),10*7);
  }
  {
    auto pred = mName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(atoms.filter(pred).size(),10*7);
  }
  {
    auto pred = mName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(atoms.filter(pred).size(),30*7);
  }
  {
    auto pred = mName.is_in(std::set<MoleculeName>{MoleculeName{"A"}});
    EXPECT_EQ(atoms.filter(pred).size(),10*7);
  }

  {
    EXPECT_EQ(residues.filter(mName=="A").size(),10);
  }
  {
    EXPECT_EQ(residues.filter(mName!="A").size(),20);
  }
  {
    EXPECT_EQ(residues.filter(mName==MoleculeName("B")).size(),20);
  }
  {
    EXPECT_EQ(residues.filter(mName!=MoleculeName("B")).size(),10);
  }
  {
    EXPECT_EQ(residues.filter(mName==std::string("A")).size(),10);
  }
  {
    EXPECT_EQ(residues.filter(mName!=std::string("B")).size(),10);
  }
  {
    auto pred = mName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(residues.filter(pred).size(),10);
  }
  {
    auto pred = mName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(residues.filter(pred).size(),30);
  }
  {
    auto pred = mName.is_in(std::set<MoleculeName>{MoleculeName{"A"}});
    EXPECT_EQ(residues.filter(pred).size(),10);
  }

}


