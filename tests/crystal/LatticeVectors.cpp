#include <gtest/gtest.h>

#include "xmol/crystal/LatticeVectors.h"
#include "xmol/geometry/AngleValue.h"
#include "xmol/geometry/XYZ.h"

using ::testing::Test;
using namespace xmol::geometry;
using namespace xmol::crystal;

class lattice_vectors_Tests : public Test {
public:
};

TEST_F(lattice_vectors_Tests, construction) {
  UnitCell latticeVectors(1, 1, 1, Degrees(90), Degrees(90), Degrees(90));

  EXPECT_DOUBLE_EQ(latticeVectors[0].x(), 1);
  EXPECT_DOUBLE_EQ(latticeVectors[1].y(), 1);
  EXPECT_DOUBLE_EQ(latticeVectors[2].z(), 1);

  latticeVectors.scale_by(10);

  EXPECT_DOUBLE_EQ(latticeVectors[0].x(), 10);
  EXPECT_DOUBLE_EQ(latticeVectors[1].y(), 10);
  EXPECT_DOUBLE_EQ(latticeVectors[2].z(), 10);

  UnitCell latticeVectors2(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));
}

TEST_F(lattice_vectors_Tests, best_shift) {
  UnitCell latticeVectors(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));

  auto ref = XYZ(0, 0, 0);
  auto var = XYZ(1, 2, 3);

  auto image = latticeVectors.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(-1, image.shift.x());
  EXPECT_DOUBLE_EQ(-2, image.shift.y());
  EXPECT_DOUBLE_EQ(-3, image.shift.z());
}

TEST_F(lattice_vectors_Tests, best_shift2) {

  UnitCell latticeVectors(XYZ(1, 4, 1), XYZ(5, 1, 1), XYZ(7, 1, 4));

  auto ref = XYZ(0, 0, 0);
  auto var = ref + latticeVectors.translation_vector(6, -1, 4);

  auto image = latticeVectors.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(ref.x(), image.pos.x());
  EXPECT_DOUBLE_EQ(ref.y(), image.pos.y());
  EXPECT_DOUBLE_EQ(ref.z(), image.pos.z());
}

TEST_F(lattice_vectors_Tests, best_shift3){

  UnitCell latticeVectors(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));

  latticeVectors.scale_by(1.5);

  auto ref = XYZ(0, 0, 0);
  auto var = ref + latticeVectors.translation_vector(1, 2, 3);

  auto image = latticeVectors.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(-1.5, image.shift.x());
  EXPECT_DOUBLE_EQ(-3.0, image.shift.y());
  EXPECT_DOUBLE_EQ(-4.5, image.shift.z());
}