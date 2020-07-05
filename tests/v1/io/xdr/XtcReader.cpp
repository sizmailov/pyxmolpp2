#include "xmol/io/xdr/XtcReader.h"
#include "xmol/future/span.h"
#include <gtest/gtest.h>

using ::testing::Test;
using namespace xmol::io::xdr;
using namespace xmol;

class XdrReaderTests : public Test {};

TEST_F(XdrReaderTests, sound) {
  std::string filename = "gromacs/xtc/1am7_corrected.xtc";
  XtcReader reader(filename);
  XtcHeader header{};
  std::array<float, 9> box{};
  std::vector<float> coords{};
  int frame_idx = 0;
  while (true) {
    if (!reader.read_header(header)) {
      break;
    }
    coords.resize(header.n_atoms * 3);
    if (!reader.read_box(box)) {
      break;
    }
    if (!reader.read_coords(coords)) {
      break;
    };
    ++frame_idx;
  }
  EXPECT_EQ(frame_idx, 51);
}
