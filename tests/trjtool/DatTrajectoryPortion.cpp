#include <gtest/gtest.h>

#include "xmol/pdb/PDBReader.h"
#include "xmol/pdb/PDBRecord.h"
#include "xmol/trajectory/Trajectory.h"
#include "xmol/trjtool/DatTajectoryPortion.h"
#include <fstream>

using ::testing::Test;
using namespace xmol::trjtool;
using namespace xmol::trajectory;

class TrjtoolDatPortionTests : public Test {
public:
  TrjtoolDatPortionTests()
      : permissibleRecords(xmol::pdb::StandardPdbRecords::instance()) {
    permissibleRecords.alter_record(xmol::pdb::RecordTypeName("ATOM"),
                                    xmol::pdb::RecordFieldName("serial"),
                                    {7, 12});
  }

  xmol::pdb::AlteredPdbRecords permissibleRecords;
};

TEST_F(TrjtoolDatPortionTests, init) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PDBReader(ifs2).read_frame(permissibleRecords);

  DatTrajectoryPortion portion("trjtool/GB1/run00001.dat");
  std::cout << portion.n_frames() << std::endl;
}

TEST_F(TrjtoolDatPortionTests, add_to_trajectory) {
  std::ifstream ifs2("trjtool/GB1/run00001.pdb", std::ios::binary);
  auto frame = xmol::pdb::PDBReader(ifs2).read_frame(permissibleRecords);
  auto frame2 = frame;

  Trajectory traj(frame);
  traj.add_trajectory_portion<DatTrajectoryPortion>("trjtool/GB1/run00001.dat");
  traj.add_trajectory_portion<DatTrajectoryPortion>("trjtool/GB1/run00002.dat");

  {
    int k = 0;
    for (auto& x : traj.slice(0, 100)) {
      k++;
    }
    EXPECT_EQ(k, 100);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(100, 0)) {
      k++;
    }
    EXPECT_EQ(k, 0);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(100, 0, -1)) {
      k++;
    }
    EXPECT_EQ(k, 100);
  }
  {
    int k = 0;
    for (auto& x : traj.slice(-100, -1, 1)) {
      k++;
    }
    EXPECT_EQ(k, 99);
  }

  {
    int k = 0;
    for (auto& x : traj.slice({}, -1)) {
      k++;
    }
    EXPECT_EQ(k, 1999);
  }
  {
    int k = 0;
    for (auto& x : traj.slice({}, {}, -1)) {
      k++;
    }
    EXPECT_EQ(k, 2000);
  }
  {
    int k = 0;
    for (auto& x : traj.slice({}, {}, {})) {
      k++;
    }
    EXPECT_EQ(k, 2000);
  }
}
