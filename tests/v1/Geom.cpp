#include <gtest/gtest.h>

#include "xmol/v1/Frame.h"
#include "xmol/v1/geom/alignment-impl.h"

using ::testing::Test;
using namespace xmol::v1;
using namespace xmol::v1::geom;
using namespace xmol::v1::geom::affine;

class GeomTests : public Test {
public:
  template <typename T> void print_shapes(T&& t, int line = 0) {
    std ::cout << line << ": shape=(" << t.rows() << ", " << t.cols() << ") " << __PRETTY_FUNCTION__ << std::endl;
  }
};

TEST_F(GeomTests, alignment) {

  double data[] = {1, 2, 3, 2, 4, 5, 8, 1, 3, 1, 1, 1, 5, 1, 2};

  CoordEigenMatrix X = CoordEigenMatrixMap(data, 3, 5);
  CoordEigenMatrix Y = X;

  Transformation3d G = Translation3d(XYZ(-1, 1, 2)) * Rotation3d(XYZ(1, 2, 3), Degrees(10));
  auto M1 = G.get_underlying_matrix();
  XYZ::Vector_t dr1 = G.get_translation()._eigen();
  Y = (M1 * Y).colwise() + dr1;

  Eigen::Matrix3d M1_inv = G.inverted().get_underlying_matrix();
  CoordEigenVector dr1_inv = G.inverted().get_translation()._eigen();

  auto G2 = calc_alignment_impl(X, Y);
  auto M2 = G2.get_underlying_matrix();
  XYZ::Vector_t dr2 = G2.get_translation()._eigen();
  Y = (M2 * Y).colwise() + dr2;
  EXPECT_LT((M2 - M1_inv).array().abs().maxCoeff(), 1e-12);
  EXPECT_LT((dr2 - dr1_inv).array().abs().maxCoeff(), 1e-12);

  EXPECT_LT((X - Y).array().abs().maxCoeff(), 1e-12);
}

TEST_F(GeomTests, calc_intertia_tensor) {
  {
    double data[] = {0, 1, 0, 1, 0, 0, -1, 0, 0, 0, -1, 0};
    CoordEigenMatrix X = CoordEigenMatrixMap(data, 3, 4);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X);
    Eigen::Matrix3d expected = XYZ(2, 2, 4)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }

  {
    double data[] = {0, 10, 0, 1, 0, 0, -1, 0, 0, 0, -10, 0};
    CoordEigenMatrixMap X(data, 3, 4);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X);
    Eigen::Matrix3d expected = XYZ(200, 2, 202)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }

  {
    double data_coord[] = {0, 1, 0, 1, 0, 0, -1, 0, 0, 0, -1, 0};
    double data_mass[] = {10, 1, 1, 10};
    CoordEigenMatrix X = CoordEigenMatrixMap(data_coord, 3, 4);
    Eigen::Map<Eigen::Matrix<double, 1, Eigen::Dynamic>> mass(data_mass, 1, 4);
    Eigen::Matrix3d tensor = calc_inertia_tensor_impl(X, mass);
    Eigen::Matrix3d expected = XYZ(20, 2, 22)._eigen().asDiagonal();
    EXPECT_DOUBLE_EQ((tensor - expected).array().abs().maxCoeff(), 0);
  }
}