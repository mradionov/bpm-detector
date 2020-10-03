#include <iostream>

#include "fourier_transform.h"

std::vector<std::complex<double>> discrete_fourier_transform(
  std::vector<double> x
) {
  size_t N = x.size();
  size_t M = N;

  std::vector<std::complex<double>> out;
  out.reserve(M);

  std::complex<double> sum;

  for (size_t m = 0; m < M; m++) {
    sum = std::complex<double>(0, 0);

    for (size_t n = 0; n < N; n++) {
      double realPart = std::cos(2 * M_PI * n * m / N);
      double imagPart = std::sin(2 * M_PI * n * m / N);

      std::complex<double> w(realPart, -imagPart);

      sum += x[n] * w;
    }

    out.push_back(sum);
  }

  return out;
}

std::vector<double> discrete_fourier_transform_amp(std::vector<double> x) {
  size_t N = x.size();
  size_t M = N;

  std::vector<double> out;
  out.reserve(M);

  std::complex<double> sum;

  for (size_t m = 0; m < M; m++) {
    sum = std::complex<double>(0, 0);

    for (size_t n = 0; n < N; n++) {
      double realPart = std::cos(2 * M_PI * n * m / N);
      double imagPart = std::sin(2 * M_PI * n * m / N);

      std::complex<double> w(realPart, -imagPart);

      sum += x[n] * w;
    }

    double amp = std::sqrt(std::pow(sum.real(), 2) + std::pow(sum.imag(), 2));

    out.push_back(amp);
  }

  return out;
}
