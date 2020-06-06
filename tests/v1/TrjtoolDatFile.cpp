#include <gtest/gtest.h>

#include <memory>

#include "xmol/v1/trajectory/Trajectory.h"
#include "xmol/v1/trajectory/TrjtoolDatFile.h"

using ::testing::Test;
using namespace xmol::v1;
using namespace xmol::v1::proxy::smart;
using namespace xmol::v1::trajectory;

class TrjtoolDatFileTests : public Test {};

TEST_F(TrjtoolDatFileTests, read) {
  std::string filename("trjtool/GB1/run00001.dat");
  TrjtoolDatFile dat_file(filename);

  EXPECT_EQ(dat_file.n_frames(), 1000);
  EXPECT_EQ(dat_file.n_atoms(), 880);

  std::vector<XYZ> coords(dat_file.n_atoms());
  auto xyz_span = future::Span(coords.data(), coords.size());
  dat_file.advance(0);
  dat_file.read_coordinates(0, xyz_span);
  XYZ expected = {8.9505224227905273, -9.4666690826416016, -3.2724499702453613};
  XYZ actual = xyz_span[0];
  EXPECT_LE((expected - actual).len(), 1e-3);
}

TEST_F(TrjtoolDatFileTests, trajectory) {
  const std::string filename("trjtool/GB1/run00001.dat");

  Frame frame;
  auto mol = frame.add_molecule({});
  auto res = mol.add_residue({}, {});
  const int n = 880;
  for (int i = 0; i < n; ++i) {
    res.add_atom({}, {});
  }
  Trajectory traj(std::move(frame));
  EXPECT_EQ(traj.n_atoms(), 880);
  traj.extend(TrjtoolDatFile(filename));
  EXPECT_EQ(traj.n_frames(), 1000);
  traj.extend(TrjtoolDatFile(filename));
  EXPECT_EQ(traj.n_frames(), 2000);
  int count = 0;
  for (auto& _: traj){
    count +=1;
    static_cast<void>(_);
  }
  EXPECT_EQ(count, 2000);

}
