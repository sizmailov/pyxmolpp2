#include <gtest/gtest.h>

#include <memory>

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
