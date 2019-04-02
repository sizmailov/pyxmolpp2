#include <gtest/gtest.h>

#include "xmol/geometry/calculate_sasa.h"

using ::testing::Test;
using namespace xmol::geometry;

class calculate_sasa_Tests : public Test {
public:
};

TEST_F(calculate_sasa_Tests, test1)
{
  std::vector<double> radii = { 1.0, 1.0};
  std::vector<XYZ> coords = { XYZ(0,0,0), XYZ(0,0,1) };
  auto result = calculate_sasa(coords, radii, 0);
  EXPECT_EQ(result.size(), coords.size());
  EXPECT_DOUBLE_EQ(result[0], result[1]);
}
