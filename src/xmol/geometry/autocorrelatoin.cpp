#include "xmol/geometry/autocorrelation.h"

#include "unsupported/Eigen/FFT"
#include <iostream>

using namespace xmol::geometry;

namespace {


void calc_Y22(const std::vector<XYZ>& v, std::vector<std::complex<double>>& out){
  auto coeff = 1.0/4.0*std::sqrt(15.0/2.0/M_PI);
  for (size_t i=0;i<v.size();++i){
    auto r = v[i]/v[i].len();
    out[i] = coeff*std::complex<double>(r.x(),r.y())*std::complex<double>(r.x(),r.y());
  }
}

void calc_Y21(const std::vector<XYZ>& v, std::vector<std::complex<double>>& out){
  auto coeff = -1.0/2.0*std::sqrt(15.0/2.0/M_PI);
  for (size_t i=0;i<v.size();++i){
    auto r = v[i]/v[i].len();
    out[i] = coeff*std::complex<double>(r.x(),r.y())*r.z();
  }
}
void calc_Y20(const std::vector<XYZ>& v, std::vector<std::complex<double>>& out){
  auto coeff = 1.0/4.0*std::sqrt(5.0/M_PI);
  for (size_t i=0;i<v.size();++i){
    auto r = v[i]/v[i].len();
    out[i] = coeff*(2*r.z()*r.z()-r.x()*r.x() - r.y()*r.y());
  }
}


void autocorr(std::vector<std::complex<double>>& series, std::vector<std::complex<double>>& tmp){

  Eigen::FFT<double> fft;

  fft.fwd(tmp, series);
  for (auto&x : tmp){
    x *= std::conj(x);
  }
  fft.inv(series, tmp);

}

}
std::vector<double> xmol::geometry::calc_autocorr_order_2(const std::vector<xmol::geometry::XYZ>& v
                                                          , long long int limit
                                                         ) {

  long long N = v.size();
  if (limit<0 || limit>v.size()){
    limit = v.size();
  }

  std::vector<double> result(limit,0.0);

  std::vector<std::complex<double>> Y(2*N);
  std::vector<std::complex<double>> tmp(2*N);

  std::fill(Y.begin()+N,Y.end(),0);

  ::calc_Y20(v,Y);
  ::autocorr(Y,tmp);

  for (long long i=0;i<limit;i++){
    result[i] += Y[i].real();
  }

  std::fill(Y.begin()+N,Y.end(),0);
  ::calc_Y21(v,Y);
  ::autocorr(Y,tmp);

  for (long long i=0;i<limit;i++){
    result[i] += 2*Y[i].real();
  }

  std::fill(Y.begin()+N,Y.end(),0);
  ::calc_Y22(v,Y);
  ::autocorr(Y,tmp);

  for (long long i=0;i<limit;i++){
    result[i] += 2*Y[i].real();
  }

  for (long long i=0;i<limit;i++){
    result[i] *= 4*M_PI/5.0/(N-i);
  }

  return result;
}