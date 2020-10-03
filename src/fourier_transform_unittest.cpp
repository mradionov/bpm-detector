#include <iostream>

#include "../extern/catch/catch2.hpp"

#include "fourier_transform.h"

TEST_CASE("book example 1") {
  using vecsz = std::vector<double>::size_type;

  int fs = 8000; // sampling frequency
  vecsz N = 8;

  std::vector<double> x(N);

  for (vecsz n = 0; n < N; n++) {
    x[n] = std::sin(2 * M_PI * 1000 * n * (1.0 / fs))
      + 0.5 * std::sin(2 * M_PI * 2000 * n * (1.0 / fs) + 3 * M_PI / 4);
    std::cout << x[n] << "\n";
  }

  REQUIRE(x[0] == Approx(0.3535).margin(0.0001));
  REQUIRE(x[2] == Approx(0.6464).margin(0.0001));
  REQUIRE(x[5] == Approx(-1.0607).margin(0.0001));

  std::cout << "---" << "\n";

  std::vector<double> out = discrete_fourier_transform_amp(x);

  for (vecsz i = 0; i < out.size(); i++) {
    std::cout << out[i] << "\n";
  }

  REQUIRE(out.size() == x.size());
  REQUIRE(out[0] == Approx(0).margin(0.0001));
  REQUIRE(out[1] == Approx(4).margin(0.0001));
  REQUIRE(out[2] == Approx(2).margin(0.0001));
  REQUIRE(out[3] == Approx(0).margin(0.0001));
  REQUIRE(out[4] == Approx(0).margin(0.0001));
  REQUIRE(out[5] == Approx(0).margin(0.0001));
  REQUIRE(out[6] == Approx(2).margin(0.0001));
  REQUIRE(out[7] == Approx(4).margin(0.0001));
}
