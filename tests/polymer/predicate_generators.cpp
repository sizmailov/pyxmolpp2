#include <gtest/gtest.h>

#include "xmol/polymer/predicate_generators.h"

using ::testing::Test;
using namespace xmol::polymer;



class PredicateGeneratorsTests : public Test{
public:
  Frame make_polyglycines(const std::vector<std::pair<std::string, int>>& chain_sizes) const {
    Frame frame(0);
    int aid=1;
    int rid=1;
    for (auto& chN_S: chain_sizes ){
      Chain& c = frame.emplace(ChainName(chN_S.first));
      for (int i=0;i<chN_S.second;i++){
        Residue &r = c.emplace(ResidueName("GLY"),residueId_t(rid++));
        r.emplace(AtomName("N"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("H"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("CA"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("HA2"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("HA3"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("C"),atomId_t(aid++),XYZ{1,2,3});
        r.emplace(AtomName("O"),atomId_t(aid++),XYZ{1,2,3});
      }
    }
    return frame;
  }
};



TEST_F(PredicateGeneratorsTests, test_atom_name_predicate){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto atoms = frame.asAtoms();

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

  auto residues = frame.asResidues();

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

  auto chains = frame.asChains();

  {
    EXPECT_EQ(chains.filter(cName=="A").size(),1);
  }
  {
    EXPECT_EQ(chains.filter(cName!="A").size(),1);
  }
  {
    EXPECT_EQ(chains.filter(cName==ChainName("B")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(cName!=ChainName("B")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(cName==std::string("A")).size(),1);
  }
  {
    EXPECT_EQ(chains.filter(cName!=std::string("B")).size(),1);
  }
  {
    auto pred = cName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(chains.filter(pred).size(),1);
  }
  {
    auto pred = cName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(chains.filter(pred).size(),2);
  }
  {
    auto pred = cName.is_in(std::set<ChainName>{ChainName{"A"}});
    EXPECT_EQ(chains.filter(pred).size(),1);
  }

}

TEST_F(PredicateGeneratorsTests, test_chain_name_predicate_on_atoms_and_residues){
  Frame frame = make_polyglycines({{"A",10},{"B",20}});

  auto atoms = frame.asAtoms();

  auto residues = frame.asResidues();

  {
    EXPECT_EQ(atoms.filter(cName=="A").size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(cName!="A").size(),20*7);
  }
  {
    EXPECT_EQ(atoms.filter(cName==ChainName("B")).size(),20*7);
  }
  {
    EXPECT_EQ(atoms.filter(cName!=ChainName("B")).size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(cName==std::string("A")).size(),10*7);
  }
  {
    EXPECT_EQ(atoms.filter(cName!=std::string("B")).size(),10*7);
  }
  {
    auto pred = cName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(atoms.filter(pred).size(),10*7);
  }
  {
    auto pred = cName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(atoms.filter(pred).size(),30*7);
  }
  {
    auto pred = cName.is_in(std::set<ChainName>{ChainName{"A"}});
    EXPECT_EQ(atoms.filter(pred).size(),10*7);
  }

  {
    EXPECT_EQ(residues.filter(cName=="A").size(),10);
  }
  {
    EXPECT_EQ(residues.filter(cName!="A").size(),20);
  }
  {
    EXPECT_EQ(residues.filter(cName==ChainName("B")).size(),20);
  }
  {
    EXPECT_EQ(residues.filter(cName!=ChainName("B")).size(),10);
  }
  {
    EXPECT_EQ(residues.filter(cName==std::string("A")).size(),10);
  }
  {
    EXPECT_EQ(residues.filter(cName!=std::string("B")).size(),10);
  }
  {
    auto pred = cName.is_in(std::set<const char*>{"A"});
    EXPECT_EQ(residues.filter(pred).size(),10);
  }
  {
    auto pred = cName.is_in(std::set<std::string>{"A","B"});
    EXPECT_EQ(residues.filter(pred).size(),30);
  }
  {
    auto pred = cName.is_in(std::set<ChainName>{ChainName{"A"}});
    EXPECT_EQ(residues.filter(pred).size(),10);
  }

}


