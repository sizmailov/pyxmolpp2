#include <gtest/gtest.h>

#include "xmol/io/AmberNetCDF.h"
#include "xmol/io/PdbInputFile.h"
#include "xmol/trajectory/Trajectory.h"

using ::testing::Test;
using namespace xmol::io;
using namespace xmol;

class AmberNetCDFTrajectoryFileTests : public Test {
public:
  AmberNetCDFTrajectoryFileTests() {
    auto pdb_file = PdbInputFile(pdb_filename);
    assert(pdb_file.n_frames()>0);
    assert(pdb_file.n_atoms()>0);
    frame = pdb_file.frames()[0];
  }

  const char* const nc_filename = "amber/GB1_F30C_MTSL/GB1_F30C_MTSL_10_frames.nc";
  const char* const pdb_filename = "amber/GB1_F30C_MTSL/box.pdb";
  Frame frame;
};

TEST_F(AmberNetCDFTrajectoryFileTests, constructor) { AmberNetCDF nc(nc_filename); }

TEST_F(AmberNetCDFTrajectoryFileTests, read_dimensions) {
  AmberNetCDF nc(nc_filename);

  EXPECT_EQ(nc.n_atoms(), 10470);
  EXPECT_EQ(nc.n_frames(), 10);
}

TEST_F(AmberNetCDFTrajectoryFileTests, read_coordinates) {
  AmberNetCDF nc(nc_filename);
  auto coords = frame.coords();
  nc.read_coordinates(0, coords);
}

//TEST_F(AmberNetCDFTrajectoryFileTests, prints) {
//  NetCDFTrajectoryFile nc(nc_filename);
//  nc.print_info();
//}

TEST_F(AmberNetCDFTrajectoryFileTests, trajectory) {
  ;
  trajectory::Trajectory traj(frame);
  traj.extend(AmberNetCDF(nc_filename));

  EXPECT_EQ(traj.n_frames(), 10);

  //    traj.set_update_list(ref.asChains()[0].asAtoms());

  for (auto& frame : traj) {
    static_cast<void>(frame.index);
    //        std::cout << frame.index() << std::endl;
    //        frame.to_pdb("frames/"+std::to_string(frame.index())+".pdb");
  }
}