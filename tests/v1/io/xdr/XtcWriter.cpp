#include "xmol/io/xdr/XtcWriter.h"
#include "xmol/Frame.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/io/PdbInputFile.h"
#include <gtest/gtest.h>

using ::testing::Test;
using namespace xmol::io::xdr;
using namespace xmol;

class XtcWriterTests : public Test {};

TEST_F(XtcWriterTests, write_trajectory) {
  std::string filename = "temp.xtc";
  XtcWriter writer(filename, 1000);

  Frame frame = io::PdbInputFile("pdb/rcsb/1PGB.pdb").frames()[0];
  for (int i = 0; i < 10; i++) {
    frame.coords().apply(geom::affine::Translation3d(XYZ(0.5, 0, 0)));
    frame.cell.scale_by(1.2);
    writer.write(frame);
  }
}
