#include <gtest/gtest.h>

#include "xmol/Frame.h"
#include "xmol/algo/alignment-impl.h"
#include "xmol/geom/UnitCell.h"

using ::testing::Test;
using namespace xmol;
using namespace xmol::algo;
using namespace xmol::geom;
using namespace xmol::geom::affine;

class GeomTests : public Test {
public:
  template <typename T> void print_shapes(T&& t, int line = 0) {
    std ::cout << line << ": shape=(" << t.rows() << ", " << t.cols() << ") " << __PRETTY_FUNCTION__ << std::endl;
  }
};

TEST_F(GeomTests, alignment) {

  double data[] = {1, 2, 3, 2, 4, 5, 8, 1, 3, 1, 1, 1, 5, 1, 2};

  CoordEigenMatrix X = CoordEigenMatrixMap(data, 5, 3);
  CoordEigenMatrix Y = X;

  Transformation3d G = Translation3d(XYZ(-1, 1, 2)) * Rotation3d(XYZ(1, 2, 3), Degrees(10));
  auto M1 = G.get_underlying_matrix();
  XYZ::Vector_t dr1 = G.get_translation()._eigen();
  Y = (M1 * Y.transpose()).transpose().rowwise() + dr1;

  Eigen::Matrix3d M1_inv = G.inverted().get_underlying_matrix();
  CoordEigenVector dr1_inv = G.inverted().get_translation()._eigen();

  auto G2 = calc_alignment_impl(X, Y);
  auto M2 = G2.get_underlying_matrix();
  XYZ::Vector_t dr2 = G2.get_translation()._eigen();
  Y = (M2 * Y.transpose()).transpose().rowwise() + dr2;
  EXPECT_LT((M2 - M1_inv).array().abs().maxCoeff(), 1e-12);
  EXPECT_LT((dr2 - dr1_inv).array().abs().maxCoeff(), 1e-12);

  EXPECT_LT((X - Y).array().abs().maxCoeff(), 1e-12);
}

TEST_F(GeomTests, calc_intertia_tensor) {
  {
    double data[] = {0, 1, 0, 1, 0, 0, -1, 0, 0, 0, -1, 0};
    CoordEigenMatrix X = CoordEigenMatrixMap(data, 4, 3);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X);
    Eigen::Matrix3d expected = XYZ(2, 2, 4)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }

  {
    double data[] = {0, 10, 0, 1, 0, 0, -1, 0, 0, 0, -10, 0};
    CoordEigenMatrixMap X(data, 4, 3);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X);
    Eigen::Matrix3d expected = XYZ(200, 2, 202)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }

  {
    double data_coord[] = {0, 1, 0, 1, 0, 0, -1, 0, 0, 0, -1, 0};
    double data_mass[] = {10, 1, 1, 10};
    CoordEigenMatrix X = CoordEigenMatrixMap(data_coord, 4, 3);
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> mass(data_mass, 4, 1);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X, mass);
    Eigen::Matrix3d expected = XYZ(20, 2, 22)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }
}


TEST_F(GeomTests, construction) {
  UnitCell cell(1, 1, 1, Degrees(90), Degrees(90), Degrees(90));

  EXPECT_DOUBLE_EQ(cell[0].x(), 1);
  EXPECT_DOUBLE_EQ(cell[1].y(), 1);
  EXPECT_DOUBLE_EQ(cell[2].z(), 1);

  cell.scale_by(10);

  EXPECT_DOUBLE_EQ(cell[0].x(), 10);
  EXPECT_DOUBLE_EQ(cell[1].y(), 10);
  EXPECT_DOUBLE_EQ(cell[2].z(), 10);

  UnitCell cell2(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));
}

TEST_F(GeomTests, best_shift) {
  UnitCell cell(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));

  auto ref = XYZ(0, 0, 0);
  auto var = XYZ(1, 2, 3);

  auto image = cell.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(-1, image.shift.x());
  EXPECT_DOUBLE_EQ(-2, image.shift.y());
  EXPECT_DOUBLE_EQ(-3, image.shift.z());
}

TEST_F(GeomTests, best_shift2) {

  UnitCell cell(XYZ(1, 4, 1), XYZ(5, 1, 1), XYZ(7, 1, 4));

  auto ref = XYZ(0, 0, 0);
  auto var = ref + cell.translation_vector(6, -1, 4);

  auto image = cell.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(ref.x(), image.pos.x());
  EXPECT_DOUBLE_EQ(ref.y(), image.pos.y());
  EXPECT_DOUBLE_EQ(ref.z(), image.pos.z());
}

TEST_F(GeomTests, best_shift3){
  UnitCell cell(XYZ(1, 0, 0), XYZ(0, 1, 0), XYZ(0, 0, 1));

  cell.scale_by(1.5);

  auto ref = XYZ(0, 0, 0);
  auto var = ref + cell.translation_vector(1, 2, 3);

  auto image = cell.closest_image_to(ref, var);

  EXPECT_DOUBLE_EQ(image.distance, 0);
  EXPECT_DOUBLE_EQ(-1.5, image.shift.x());
  EXPECT_DOUBLE_EQ(-3.0, image.shift.y());
  EXPECT_DOUBLE_EQ(-4.5, image.shift.z());
}