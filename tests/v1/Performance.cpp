#include <gtest/gtest.h>

#include "xmol/polymer/Atom.h"
#include "xmol/v1/Frame.h"

#include <chrono>

using ::testing::Test;
using namespace xmol;

class PerformanceTests : public Test {

  void SetUp() override {}

public:
  const int n_molecules = 1000;
  const int n_residues_per_molecule = 100;
  const int n_atoms_per_residue = 30;

  v1::Frame create_frame_v1() {
    v1::Frame frame;
    frame.reserve_molecules(n_molecules);
    frame.reserve_residues(n_molecules * n_residues_per_molecule);
    frame.reserve_atoms(n_molecules * n_residues_per_molecule * n_atoms_per_residue);

    for (int i = 0; i < n_molecules; ++i) {
      v1::MoleculeSmartRef molecule = frame.add_molecule(v1::MoleculeName(""));
      for (int j = 0; j < n_residues_per_molecule; ++j) {
        v1::ResidueSmartRef residue = molecule.add_residue(v1::ResidueName(""), v1::ResidueId(0));
        for (int k = 0; k < n_atoms_per_residue; ++k) {
          residue.add_atom(v1::AtomName(""), v1::AtomId(0));
        }
      }
    }
    return frame;
  };

  polymer::Frame create_frame_v0() {
    polymer::Frame frame(0, n_molecules);

    for (int i = 0; i < n_molecules; ++i) {
      auto& chain = frame.emplace(polymer::ChainName(""), n_residues_per_molecule);
      for (int j = 0; j < n_residues_per_molecule; ++j) {
        auto& residue = chain.emplace(polymer::ResidueName(""), v1::ResidueId(0), n_atoms_per_residue);
        for (int k = 0; k < n_atoms_per_residue; ++k) {
          residue.emplace(polymer::AtomName(""), v1::AtomId(0), polymer::XYZ{});
        }
      }
    }
    return frame;
  };

  using DeltaT = decltype(std::chrono::high_resolution_clock::now()-std::chrono::high_resolution_clock::now());

  auto to_us(const DeltaT& dt){
    return std::chrono::duration_cast<std::chrono::microseconds>(dt);
  }
};

TEST_F(PerformanceTests, frame_forward_construction) {

  auto t1 = std::chrono::high_resolution_clock::now();
  auto v0 = create_frame_v0();
  auto t2 = std::chrono::high_resolution_clock::now();
  auto v1 = create_frame_v1();
  auto t3 = std::chrono::high_resolution_clock::now();

  const int w1 = 25;
  const int w = 10;
  std::cout << std::setw(w1) << n_molecules << " molecules" << std::endl;
  std::cout << std::setw(w) << n_molecules*n_residues_per_molecule << " residues ("<< n_residues_per_molecule<<" per molecule)" << std::endl;
  std::cout << std::setw(w) << n_molecules*n_residues_per_molecule*n_atoms_per_residue << " atoms ("<< n_atoms_per_residue<<" per residue)" << std::endl;

  std::cout << std::setw(w1) << "" << std::setw(w) << "v0" << std::setw(w) << "v1" << std::endl;

  std::cout
      << std::setw(w1) << "create"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  auto chains_v0 = v0.asChains();
  t2 = std::chrono::high_resolution_clock::now();
  auto chains_v1 = v1.molecules();
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ(chains_v0.size(), chains_v1.size());

  std::cout
      << std::setw(w1) << "asChains"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  auto residues_v0 = v0.asResidues();
  t2 = std::chrono::high_resolution_clock::now();
  auto residues_v1 = v1.residues();
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ(residues_v0.size(), residues_v1.size());

  std::cout
      << std::setw(w1) << "asResidues"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  auto atoms_v0 = v0.asAtoms();
  t2 = std::chrono::high_resolution_clock::now();
  auto atoms_v1 = v1.atoms();
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ(atoms_v0.size(), atoms_v1.size());

  std::cout
      << std::setw(w1) << "asAtoms"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  for (auto& a: atoms_v0){
    a.set_r(polymer::XYZ{1,2,3});
  }
  t2 = std::chrono::high_resolution_clock::now();
  for (auto& a: atoms_v1){
    a.r(polymer::XYZ{1,2,3});
  }
  t3 = std::chrono::high_resolution_clock::now();

  std::cout
      << std::setw(w1) << "assign atom.r"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  polymer::XYZ sum0;
  for (auto& a: atoms_v0){
    sum0 += a.r();
  }
  t2 = std::chrono::high_resolution_clock::now();
  polymer::XYZ sum1;
  for (auto& a: atoms_v1){
    sum1 += a.r();
  }
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ((sum0-sum1).len(),0);

  std::cout
      << std::setw(w1) << "sum atom.r"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  for (auto& a: atoms_v0){
    a.set_id(15);
  }
  t2 = std::chrono::high_resolution_clock::now();
  for (auto& a: atoms_v1){
    a.id(15);
  }
  t3 = std::chrono::high_resolution_clock::now();

  std::cout
      << std::setw(w1) << "assign atom.id"
      << std::setw(w) << to_us(t2-t1).count()
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  polymer::XYZ sum11;
  for (auto& r: v1.coordinates()){
    sum11 += r;
  }
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ((sum0-sum11).len(),0);

  std::cout
      << std::setw(w1) << "sum coords"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  for (auto& r: v1.coordinates()){
    r = v1::XYZ(1,2,3);
  }
  t3 = std::chrono::high_resolution_clock::now();

  std::cout
      << std::setw(w1) << "assign coords"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  std::vector<v1::proxy::AtomRef> proxy_vector;
  proxy_vector.reserve(atoms_v1.size());
  for (auto& a : atoms_v1) {
    proxy_vector.push_back(a);
  }

  t3 = std::chrono::high_resolution_clock::now();

  std::cout
      << std::setw(w1) << "sel ctor baseline"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  v1::XYZ sum4;
  for (auto& a: proxy_vector){ sum4 += a.r();}
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ((sum0-sum4).len(),0);
  std::cout
      << std::setw(w1) << "sel atom.r sum baseline"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  v1::Selection<v1::proxy::AtomRef> selection(proxy_vector);
  t3 = std::chrono::high_resolution_clock::now();

  std::cout
      << std::setw(w1) << "sel copy ctor"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

  t1 = std::chrono::high_resolution_clock::now();
  // no equivalent
  t2 = std::chrono::high_resolution_clock::now();
  v1::XYZ sum5;
  for (auto& a: selection){ sum5 += a.r();}
  t3 = std::chrono::high_resolution_clock::now();

  ASSERT_EQ((sum0 - sum5).len(), 0);
  std::cout
      << std::setw(w1) << "sel assign"
      << std::setw(w) << "n/a"
      << std::setw(w) << to_us(t3-t2).count()
      << std::endl;

  ///////////////

}