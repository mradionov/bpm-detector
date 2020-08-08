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
    if (m_min < val) {
      m_min = val;
    }
    if (m_max > val) {
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

  // 8-bit samples are stored as unsigned bytes, ranging from 0 to 255
  // 16-bit samples are stored as 2's-complement signed integers,
  // ranging from -32768 to 32767
  constexpr auto max_amplitude = std::numeric_limits<int16_t>::max();

  // Ratio how much original data should be shrinked, because all of the
  // data in 1:1 ratio probably won't fit
  constexpr size_t samples_per_group = 50;
  constexpr auto amplitude_divider = 100;

  // Divide width by 2 because two channels will be displayed one under another
  // Multiply height by 2 for the same reason.
  // Last group most likely won't be full, make sure not to lose samples from
  // it though.
  const size_t image_width = std::ceil(
    static_cast<double>(wave.data_size) / samples_per_group / 2
  );
  const size_t image_height = std::ceil(
    max_amplitude * 2 / amplitude_divider
  ) * 2;

  PpmImage image(image_width, image_height);

  SampleGroup<int16_t> left_group;
  SampleGroup<int16_t> right_group;

  size_t group_index = 0;

  std::cout << "image_width: " << image_width << std::endl;
  std::cout << "image_height: " << image_height << std::endl;

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

    if (group_index == samples_per_group) {
      // Resize it according to image, so it will take half of the height
      const auto left_top = image_height / 4
        + left_group.min() / amplitude_divider * -1;
      const auto left_bottom = image_height / 4
        + left_group.max() / amplitude_divider * -1;

      const auto right_top = image_height / 4 * 3
        + right_group.min() / amplitude_divider * -1;
      const auto right_bottom = image_height / 4 * 3
        + right_group.max() / amplitude_divider * -1;

      for (size_t image_y = 0; image_y < image_height; ++image_y) {
        if (image_y >= left_top && image_y <= left_bottom) {
          image.set_color(255, 0, 0);
          image.draw_pixel(image_x, image_y);
        } else if (image_y >= right_top && image_y <= right_bottom) {
          image.set_color(255, 0, 0);
          image.draw_pixel(image_x, image_y);
        } else {
          image.set_color(255, 255, 255);
          image.draw_pixel(image_x, image_y);
        }

        if (image_y == image_height / 2) {
          image.set_color(0, 0, 0);
          image.draw_pixel(image_x, image_y);
        }
      }

      left_group.reset();
      right_group.reset();
      image_x++;
      group_index = 0;
    }
  }

  image_file << image;
}
