#include <gtest/gtest.h>

#include <memory>

#include "xmol/trajectory/Trajectory.h"
#include "xmol/io/TrjtoolDatFile.h"
#include "xmol/io/PdbInputFile.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::proxy::smart;
using namespace xmol::trajectory;

class TrjtoolDatFileTests : public Test {
public:
  static Trajectory construct_trajectory() {
    const std::string filename("trjtool/GB1/run00001.dat");
    Frame frame;
    auto mol = frame.add_molecule();
    auto res = mol.add_residue();
    const int n = 880;
    for (int i = 0; i < n; ++i) {
      res.add_atom();
    }
    Trajectory traj(std::move(frame));
    traj.extend(io::TrjtoolDatFile(filename));
    traj.extend(io::TrjtoolDatFile(filename));
    return traj;
  }
};

TEST_F(TrjtoolDatFileTests, read) {
  std::string filename("trjtool/GB1/run00001.dat");
  io::TrjtoolDatFile dat_file(filename);

  EXPECT_EQ(dat_file.n_frames(), 1000);
  EXPECT_EQ(dat_file.n_atoms(), 880);

  std::vector<XYZ> coords(dat_file.n_atoms());
  Frame frame = io::PdbInputFile("trjtool/GB1/run00001.pdb", io::PdbInputFile::Dialect::AMBER_99).frames()[0];
  auto xyz_span = frame.coords();
  dat_file.advance(0);
  dat_file.read_frame(0, frame);
  XYZ expected = {8.9505224227905273, -9.4666690826416016, -3.2724499702453613};
  XYZ actual = xyz_span[0];
  EXPECT_LE((expected - actual).len(), 1e-3);
}

TEST_F(TrjtoolDatFileTests, trajectory_traverse) {
  Trajectory traj = construct_trajectory();
  {
    int count = 0;
    for (auto& frame : traj) {
      EXPECT_EQ(frame.index, count);
      count += 1;
    }
    EXPECT_EQ(count, 2000) << "traj";
  }
  {
    int count = 0;
    for (auto& _ : traj.slice(100)) {
      count += 1;
      static_cast<void>(_);
    }
    EXPECT_EQ(count, 1900) << "traj[100:]";
  }
  {
    int count = 0;
    for (auto& _ : traj.slice({},{},50)) {
      count += 1;
      static_cast<void>(_);
    }
    EXPECT_EQ(count, 40) << "traj[::50]";
  }
  {
    int count = 0;
    for (auto& _ : traj.slice()) {
      count += 1;
      static_cast<void>(_);
    }
    EXPECT_EQ(count, 2000) << "traj[:]";
  }
  {
    int count = 0;
    for (auto& _ : traj.slice(100, {}, 2)) {
      count += 1;
      static_cast<void>(_);
    }
    EXPECT_EQ(count, 950) << "traj[100::2]";
  }
  for (int i = 0; i < 3; i++) {
    int count = 0;
    for (auto& _ : traj.slice(100, 200, 2)) {
      count += 1;
      if (count == 25) {
        break;
      }
      static_cast<void>(_);
    }
    EXPECT_EQ(count, 25) << "traj[100:200:2] with break";
  }

  for (int i = 0; i < 3; i++) {
    int count = 0;
    try {
      for (auto& _ : traj.slice(100, 200, 2)) {
        count += 1;
        if (count == 25) {
          throw std::runtime_error("error");
        }
        static_cast<void>(_);
      }
    } catch (const std::runtime_error&) {
    }
    EXPECT_EQ(count, 25) << "traj[100:200:2] with throw";
  }
}

TEST_F(TrjtoolDatFileTests, double_penetration) {
  Trajectory traj = construct_trajectory();
  auto dp = [&traj] {
    for (auto& x : traj) {
      static_cast<void>(x);
      for (auto& y : traj) {
        static_cast<void>(y);
      }
    }
  };
  EXPECT_THROW(dp(), TrajectoryDoubleTraverseError);
}