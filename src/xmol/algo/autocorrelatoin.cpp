#include "xmol/algo/vector-correlation.h"

#include "unsupported/Eigen/FFT"
#include <iostream>

using namespace xmol::geom;
using namespace xmol::algo;

namespace {

void calc_Y22(const xmol::future::Span<xmol::geom::XYZ>& v, std::vector<std::complex<double>>& out) {
  auto coeff = 1.0 / 4.0 * std::sqrt(15.0 / 2.0 / M_PI);
  for (size_t i = 0; i < v.size(); ++i) {
    auto r = v[i] / v[i].len();
    out[i] = coeff * std::complex<double>(r.x(), r.y()) * std::complex<double>(r.x(), r.y());
  }
}

void calc_Y21(const xmol::future::Span<xmol::geom::XYZ>& v, std::vector<std::complex<double>>& out) {
  auto coeff = -1.0 / 2.0 * std::sqrt(15.0 / 2.0 / M_PI);
  for (size_t i = 0; i < v.size(); ++i) {
    auto r = v[i] / v[i].len();
    out[i] = coeff * std::complex<double>(r.x(), r.y()) * r.z();
  }
}
void calc_Y20(const xmol::future::Span<xmol::geom::XYZ>& v, std::vector<std::complex<double>>& out) {
  auto coeff = 1.0 / 4.0 * std::sqrt(5.0 / M_PI);
  for (size_t i = 0; i < v.size(); ++i) {
    auto r = v[i] / v[i].len();
    out[i] = coeff * (2 * r.z() * r.z() - r.x() * r.x() - r.y() * r.y());
  }
}

void divide_by_cube(const xmol::future::Span<xmol::geom::XYZ>& v, std::vector<std::complex<double>>& out) {
  for (size_t i = 0; i < v.size(); ++i) {
    double d = v[i].len();
    out[i] /= (d * d * d);
  }
}

void autocorr(std::vector<std::complex<double>>& series, std::vector<std::complex<double>>& tmp) {

  Eigen::FFT<double> fft;

  fft.fwd(tmp, series);
  for (auto& x : tmp) {
    x *= std::conj(x);
  }
  fft.inv(series, tmp);
}

} // namespace
void xmol::algo::calc_autocorr_order_2(const xmol::future::Span<xmol::geom::XYZ>& v, xmol::future::Span<double> result,
                                       const AutoCorrelationMode& mode) {
  const long long limit = result.size();
  long long N = v.size();

  std::vector<std::complex<double>> Y(2 * N);
  std::vector<std::complex<double>> tmp(2 * N);

  std::fill(Y.begin() + N, Y.end(), 0);

  ::calc_Y20(v, Y);
  if (mode == AutoCorrelationMode::NORMALIZE_AND_DIVIDE_BY_CUBE) {
    divide_by_cube(v, Y);
  }
  ::autocorr(Y, tmp);

  for (long long i = 0; i < limit; i++) {
    result[i] = Y[i].real();
  }

  std::fill(Y.begin() + N, Y.end(), 0);
  ::calc_Y21(v, Y);
  if (mode == AutoCorrelationMode::NORMALIZE_AND_DIVIDE_BY_CUBE) {
    divide_by_cube(v, Y);
  }
  ::autocorr(Y, tmp);

  for (long long i = 0; i < limit; i++) {
    result[i] += 2 * Y[i].real();
  }

  std::fill(Y.begin() + N, Y.end(), 0);
  ::calc_Y22(v, Y);
  if (mode == AutoCorrelationMode::NORMALIZE_AND_DIVIDE_BY_CUBE) {
    divide_by_cube(v, Y);
  }
  ::autocorr(Y, tmp);

  for (long long i = 0; i < limit; i++) {
    result[i] += 2 * Y[i].real();
  }

  for (long long i = 0; i < limit; i++) {
    result[i] *= 4 * M_PI / 5.0 / (N - i);
  }
}