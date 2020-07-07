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
    frame = PdbInputFile(pdb_filename, PdbInputFile::Dialect::AMBER_99).frames()[0];
    nve_frame = PdbInputFile(nve_pdb_filename, PdbInputFile::Dialect::AMBER_99).frames()[0];
  }

  const char* const nc_filename = "amber/GB1_F30C_MTSL/GB1_F30C_MTSL_10_frames.nc";
  const char* const pdb_filename = "amber/GB1_F30C_MTSL/box.pdb";

  const char* const nve_nc_filename = "amber/1ubq_intolerant_shake_ewald_SPCE/run00001.0-10.nc";
  const char* const nve_pdb_filename = "amber/1ubq_intolerant_shake_ewald_SPCE/box.pdb";

  Frame frame;
  Frame nve_frame;
};

TEST_F(AmberNetCDFTrajectoryFileTests, constructor) { AmberNetCDF nc(nc_filename); }

TEST_F(AmberNetCDFTrajectoryFileTests, read_dimensions) {
  {
    AmberNetCDF nc(nc_filename);
    EXPECT_EQ(nc.n_atoms(), 10470);
    EXPECT_EQ(nc.n_frames(), 10);
  }
  {
    AmberNetCDF nc(nve_nc_filename);
    EXPECT_EQ(nc.n_atoms(), 18751);
    EXPECT_EQ(nc.n_frames(), 10);
  }
}

TEST_F(AmberNetCDFTrajectoryFileTests, read_coordinates_and_cell) {
  {
    AmberNetCDF nc(nc_filename);
    EXPECT_TRUE(nc.has_cell());
    nc.read_frame(0, frame);
    EXPECT_LE(fabs(frame.cell.a() - 51.251), 1e-3);
    EXPECT_LE(fabs(frame.cell.b() - 51.251), 1e-3);
    EXPECT_LE(fabs(frame.cell.c() - 51.251), 1e-3);
    EXPECT_LE(fabs(frame.cell.alpha().degrees() - 109.471), 1e-3);
    EXPECT_LE(fabs(frame.cell.beta().degrees() - 109.471), 1e-3);
    EXPECT_LE(fabs(frame.cell.gamma().degrees() - 109.471), 1e-3);
  }
  {
    AmberNetCDF nc(nve_nc_filename);
    EXPECT_TRUE(nc.has_cell());
    nve_frame.cell = geom::UnitCell::unit_cubic_cell();
    nc.read_frame(0, nve_frame);
    EXPECT_LE(std::fabs(nve_frame.cell.volume() - 186679),  1);
  }
}

// TEST_F(AmberNetCDFTrajectoryFileTests, prints) {
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
    std::cout << frame.index << std::endl;
    std::cout << " " << frame.cell.a() << " " << frame.cell.b() << " " << frame.cell.c() << " "
              << frame.cell.alpha().degrees() << " " << frame.cell.beta().degrees() << " "
              << frame.cell.gamma().degrees() << std::endl;
    //    frame.to_pdb("frames/"+std::to_string(frame.index)+".pdb");
  }
}