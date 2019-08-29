#include <gtest/gtest.h>
#include <xmol/pdb/PdbFile.h>
#include <xmol/polymer/predicate_generators.h>
#include <xmol/pdb/PdbWriter.h>
#include <xmol/geometry/alignment.h>

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

TEST_F(AmberNetCDFTrajectoryFileTests, DISABLED_real_trajectory) {
  ;
  Frame ref = xmol::pdb::PdbFile(
      pdb_filename,
      permissibleRecords).get_frame();
  xmol::trajectory::Trajectory traj(ref);

  char buffer[] = "/home/sergei/bionmr/sergei/GB1/trj/30R1/5_run/run00001.nc";
  for (int i = 1; i < 100; i++) {
    sprintf(buffer, "/home/sergei/bionmr/sergei/GB1/trj/30R1/5_run/run%05d.nc", i);
    traj.push_trajectory_portion(NetCDFTrajectoryFile(buffer));
  }


//  EXPECT_EQ(traj.n_frames(), 4000);

  auto ref_ats = ref.asChains().filter(xmol::polymer::cIndex <= 1).asAtoms();
  traj.set_update_list(ref_ats);

//  std::ofstream ostream("frames.pdb");
//  xmol::pdb::PdbWriter out(ostream);
  {
    auto slice = traj.slice(0, traj.n_frames(), 1000);
    auto it = slice.begin();
    auto end = slice.end();
    AtomSelection asel;

    if (it != end) {
      asel = it->asChains().filter(xmol::polymer::cIndex <= 1).asAtoms();
    }

    while (it != end) {
      *it;
      auto al = xmol::geometry::calc_alignment(ref_ats.toCoords(),asel.toCoords());
      asel.for_each([&al](Atom &a) { a.set_r(al.transform(a.r())); });
//      for (auto &a: asel) {
//        out.write(a);
//      }
//      ostream << "ENDMDL\n";
      ++it;
    }
  }
//  std::cout << std::endl;
}
