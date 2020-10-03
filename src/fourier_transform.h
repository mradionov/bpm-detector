#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H

#include <complex>
#include <vector>

std::vector<std::complex<double>> discrete_fourier_transform(
  std::vector<double>
);
std::vector<double> discrete_fourier_transform_amp(std::vector<double>);

#endif
