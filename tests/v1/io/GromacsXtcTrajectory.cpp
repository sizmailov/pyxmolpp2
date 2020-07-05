#include <gtest/gtest.h>

#include "xmol/io/GromacsXtcFile.h"
#include "xmol/io/PdbInputFile.h"
#include "xmol/trajectory/Trajectory.h"

using ::testing::Test;
using namespace xmol::io;
using namespace xmol;

class GromacsXtcTrajectoryFileTests : public Test {
public:
  GromacsXtcTrajectoryFileTests() { frame = PdbInputFile(pdb_filename, PdbInputFile::Dialect::AMBER_99).frames()[0]; }

  const char* const xtc_corrected = "gromacs/xtc/1am7_corrected.xtc";
  const char* const pdb_filename = "gromacs/xtc/1am7_protein.pdb";
  const char* const pdb_first_and_last = "gromacs/xtc/1am7_first_and_last.pdb";

  Frame frame;
};

TEST_F(GromacsXtcTrajectoryFileTests, constructor) {
  GromacsXtcFile xtcFile(xtc_corrected, 51);
  EXPECT_EQ(xtcFile.n_atoms(), 2504);
  EXPECT_EQ(xtcFile.n_frames(), 51);
}

TEST_F(GromacsXtcTrajectoryFileTests, trajectory) {
  trajectory::Trajectory traj(frame);
  traj.extend(GromacsXtcFile(xtc_corrected, 51));
  EXPECT_EQ(traj.n_frames(), 51);

  for (auto& frame : traj) {
    static_cast<void>(frame.index);
  }

  auto first_and_last = PdbInputFile(pdb_first_and_last).frames();

  auto first = traj.at(0);
  for (int i = 0; i < first.atoms().size(); i++) {
    ASSERT_LE((first.atoms()[i].r()).distance(first_and_last[0].atoms()[i].r()), 1e-3)
        << first.atoms()[i].r() << " vs. " << first_and_last[0].atoms()[i].r();
  }
  auto last = traj.at(50);
  for (int i = 0; i < last.atoms().size(); i++) {
    ASSERT_LE((last.atoms()[i].r()).distance(first_and_last[1].atoms()[i].r()), 1e-3)
        << last.atoms()[i].r() << " vs. " << first_and_last[1].atoms()[i].r();
  }
}