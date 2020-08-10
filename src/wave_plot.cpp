#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include "ppm_image.h"
#include "wave_plot.h"

template <typename T>
class SampleGroup {
public:
  SampleGroup(): m_min(0), m_max(0) {}

  void apply(const T val) {
    if (val < m_min) {
      m_min = val;
    }
    if (val > m_max) {
      m_max = val;
    }
  }

  void reset() {
    m_min = 0;
    m_max = 0;
  }

  const T min() {
    return m_min;
  }

  const T max() {
    return m_max;
  }

private:
  T m_min;
  T m_max;
};

// exports PPM image (P6 binary)
// https://en.wikipedia.org/wiki/Netpbm
void wave_plot_amplitude(const WaveFile& wave, const std::string filename) {
  std::ofstream image_file(filename, std::ios::trunc);

  if (!image_file) {
    throw wave_plot_error("Cant open file");
  }

  constexpr size_t image_width = 2000;
  constexpr size_t image_height = 800;

  // 8-bit samples are stored as unsigned bytes, ranging from 0 to 255
  // 16-bit samples are stored as 2's-complement signed integers,
  // ranging from -32768 to 32767
  constexpr auto max_amplitude = std::numeric_limits<int16_t>::max();

  // Calculate how many samples are per pixel (per 1px image column)
  const auto samples_per_group = std::ceil(
    static_cast<double>(wave.data_size) / image_width / wave.num_channels
  );

  // How much amplitude will be shrinked
  const auto amplitude_divider = static_cast<double>(max_amplitude * 2)
    / image_height * 2;

  PpmImage image(image_width, image_height);
  image.set_color(255, 255, 255);
  image.draw_rect(0, 0, image_width, image_height);

  SampleGroup<int16_t> left_group;
  SampleGroup<int16_t> right_group;

  size_t group_index = 0;

  std::cout << "image_width: " << image_width << std::endl;
  std::cout << "image_height: " << image_height << std::endl;

  image.set_color(255, 0, 0);

  size_t image_x = 0;
  for (size_t i = 0; i < wave.data_size; ++i) {
    // Flip the sign because image coordinates start at top left,
    // but samples are recorded if it were at bottom left.
    if (i % 2 == 0) {
      left_group.apply(wave.data[i]);
    } else {
      right_group.apply(wave.data[i]);
      group_index++;
    }

    // Last group most likely won't be full, make sure not to lose it's samples
    if (group_index == samples_per_group || i == wave.data_size - 1) {
      // Resize it according to image, so it will take half of the height
      const auto left_top = image_height / 4
        + left_group.max() / amplitude_divider * -1;
      const auto left_bottom = image_height / 4
        + left_group.min() / amplitude_divider * -1;

      const auto right_top = image_height / 4 * 3
        + right_group.max() / amplitude_divider * -1;
      const auto right_bottom = image_height / 4 * 3
        + right_group.min() / amplitude_divider * -1;

      image.draw_column(image_x, left_top, left_bottom);
      image.draw_column(image_x, right_top, right_bottom);

      left_group.reset();
      right_group.reset();
      image_x++;
      group_index = 0;
    }
  }

  image.set_color(0, 0, 0);
  image.draw_row(image_height / 2, 0, image_width);

  image_file << image;
}
