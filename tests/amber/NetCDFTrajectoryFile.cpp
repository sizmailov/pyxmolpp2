#include <gtest/gtest.h>
#include <xmol/pdb/PdbFile.h>

#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/amber/NetCDFTrajectoryFile.h"

using ::testing::Test;
using namespace xmol::amber;
using namespace xmol::polymer;

class AmberNetCDFTrajectoryFileTests : public Test {
public:
  AmberNetCDFTrajectoryFileTests()
      :permissibleRecords(xmol::pdb::StandardPdbRecords::instance()) {
      permissibleRecords.alter_record(xmol::pdb::RecordName("ATOM"),
          xmol::pdb::FieldName("serial"),
          {7, 12});
  }
  std::vector<int> get_vector_n(int n) const {
      std::vector<int> update_list;
      for (int k = 0; k<n; k++) { update_list.push_back(k); }
      return update_list;
  }

  xmol::pdb::AlteredPdbRecords permissibleRecords;

  const char* const nc_filename = "amber/GB1_F30C_MTSL/GB1_F30C_MTSL_10_frames.nc";
  const char* const pdb_filename = "amber/GB1_F30C_MTSL/box.pdb";
};

TEST_F(AmberNetCDFTrajectoryFileTests, constructor) {
    NetCDFTrajectoryFile nc(nc_filename);

}

TEST_F(AmberNetCDFTrajectoryFileTests, read_dimensions) {
    NetCDFTrajectoryFile nc(nc_filename);

    EXPECT_EQ(nc.n_atoms_per_frame(), 10470);
    EXPECT_EQ(nc.n_frames(), 10);
}

TEST_F(AmberNetCDFTrajectoryFileTests, read_coordinates) {
    NetCDFTrajectoryFile nc(nc_filename);

    Frame frame = xmol::pdb::PdbFile(
        pdb_filename,
        permissibleRecords).get_frame();
    auto atoms = frame.asAtoms();

    auto update_list = get_vector_n(atoms.size());
    nc.set_coordinates(0, atoms, update_list);
}

TEST_F(AmberNetCDFTrajectoryFileTests, prints) {
    NetCDFTrajectoryFile nc(nc_filename);
    nc.print_info();
}

TEST_F(AmberNetCDFTrajectoryFileTests, trajectory) {
    NetCDFTrajectoryFile nc(nc_filename);
    Frame ref = xmol::pdb::PdbFile(
        pdb_filename,
        permissibleRecords).get_frame();
    xmol::trajectory::Trajectory traj(ref);
    traj.push_trajectory_portion(nc);

    EXPECT_EQ(traj.n_frames(), 10);

//    traj.set_update_list(ref.asChains()[0].asAtoms());

    for (auto& frame: traj) {
        frame.index();
//        std::cout << frame.index() << std::endl;
//        frame.to_pdb("frames/"+std::to_string(frame.index())+".pdb");
    }
}
