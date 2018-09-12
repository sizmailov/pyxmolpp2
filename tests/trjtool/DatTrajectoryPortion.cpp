#include <gtest/gtest.h>

#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/trajectory/Trajectory.h"
#include "xmol/trjtool/DatFile.h"
#include <fstream>

using ::testing::Test;
using namespace xmol::trjtool;
using namespace xmol::trajectory;

class TrjtoolDatPortionTests : public Test {
public:
  TrjtoolDatPortionTests()
      : permissibleRecords(xmol::pdb::StandardPdbRecords::instance()) {
    permissibleRecords.alter_record(xmol::pdb::RecordName("ATOM"),
                                    xmol::pdb::FieldName("serial"),
                                    {7, 12});
  }

  xmol::pdb::AlteredPdbRecords permissibleRecords;
};

TEST_F(TrjtoolDatPortionTests, init) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);

  DatFile portion("trjtool/GB1/run00001.dat");
}


TEST_F(TrjtoolDatPortionTests, basic_read_copy) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto ref = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);

  Trajectory traj(ref);
  traj.push_trajectory_portion(DatFile("trjtool/GB1/run00001.dat"));


  auto atoms = ref.asAtoms();
  EXPECT_EQ(traj.n_frames(), 1000);


  xmol::geometry::XYZ first_atom_coords (8.9505224227905273, -9.4666690826416016, -3.2724499702453613);
  EXPECT_GE((first_atom_coords-ref.asAtoms()[0].r()).len(), 1e-1);
  auto frame = *traj.begin();
  EXPECT_LE((first_atom_coords-frame.asAtoms()[0].r()).len(), 1e-3);

}

TEST_F(TrjtoolDatPortionTests, add_to_trajectory) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PdbReader(ifs2).read_frame(permissibleRecords);
  auto frame2 = frame;

  Trajectory traj(frame);
  traj.add_trajectory_portion<DatFile>("trjtool/GB1/run00001.dat");
  traj.add_trajectory_portion<DatFile>("trjtool/GB1/run00002.dat");
  traj.set_update_list(frame.asResidues()[0].asAtoms());
  {
    int k = 0;
    for (auto& x : traj.slice(0, 100)) {
      ASSERT_EQ(k,x.index());
      k++;
    }
    EXPECT_EQ(k, 100);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(100, 0)) {
      k++;
      static_cast<void>(x);
    }
    EXPECT_EQ(k, 0);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(100, 0, -1)) {
      ASSERT_EQ(100-k,x.index());
      k++;
    }
    EXPECT_EQ(k, 100);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(-100, -1, 1)) {
      ASSERT_EQ(1900+k,x.index());
      k++;
    }
    EXPECT_EQ(k, 99);
  }

  {
    int k = 0;
    for (auto& x : traj.slice({}, -1)) {
      ASSERT_EQ(k,x.index());
      k++;
    }
    EXPECT_EQ(k, 1999);
  }
  {
    int k = 0;
    for (auto& x : traj.slice({}, {}, -1)) {
      ASSERT_EQ(1999-k,x.index());
      k++;
    }
    EXPECT_EQ(k, 2000);
  }
  {
    int k = 0;
    for (auto& x : traj.slice({}, {}, {})) {
      ASSERT_EQ(k,x.index());
      k++;
    }
    EXPECT_EQ(k, 2000);
  }
}
