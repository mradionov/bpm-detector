#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
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
void wave_plot(const WaveFile& wave, std::ofstream& ofs) {
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
  const size_t image_height = std::ceil(max_amplitude * 2 / amplitude_divider)
     * 2;

  // 3 colors per pixel
  std::vector<unsigned char> image(image_height * image_width * 3);

  SampleGroup<int16_t> left_group;
  SampleGroup<int16_t> right_group;

  size_t group_index = 0;

  std::cout << "image_width: " << image_width << std::endl;
  std::cout << "image_height: " << image_height << std::endl;

  size_t image_col = 0;
  for (size_t i = 0; i < wave.data_size; ++i) {
    if (i % 2 == 0) {
      left_group.apply(wave.data[i]);
    } else {
      right_group.apply(wave.data[i]);
      group_index++;
    }

    if (group_index == samples_per_group) {
      // Resize it according to image, so it will take half of the height
      const auto left_top = image_height / 4
        + left_group.max() / amplitude_divider;
      const auto left_bottom = image_height / 4
        + left_group.min() / amplitude_divider;

      const auto right_top = image_height / 4 * 3
        + right_group.max() / amplitude_divider;
      const auto right_bottom = image_height / 4 * 3
        + right_group.min() / amplitude_divider;

      for (size_t image_row = 0; image_row < image_height; ++image_row) {
        const auto image_index = image_row * image_width * 3 + image_col * 3;

        if (image_row >= left_top && image_row <= left_bottom) {
          image[image_index] = 255;
          image[image_index + 1] = 0;
          image[image_index + 2] = 0;
        } else if (image_row >= right_top && image_row <= right_bottom) {
          image[image_index] = 255;
          image[image_index + 1] = 0;
          image[image_index + 2] = 0;
        } else {
          image[image_index] = 255;
          image[image_index + 1] = 255;
          image[image_index + 2] = 255;
        }

        if (image_row == image_height / 2) {
          image[image_index] = 0;
          image[image_index + 1] = 0;
          image[image_index + 2] = 0;
        }
      }

      left_group.reset();
      right_group.reset();
      image_col++;
      group_index = 0;
    }
  }

  // std::cout << group_index << std::endl;

  ofs << "P6\n";
  ofs << image_width << " " << image_height << "\n";
  ofs << "255\n";

  for (size_t image_row = 0; image_row < image_height; ++image_row) {
    for (size_t image_col = 0; image_col < image_width; ++image_col) {
      const auto image_index = image_row * image_width * 3 + image_col * 3;
      ofs << image[image_index];
      ofs << image[image_index + 1];
      ofs << image[image_index + 2];
    }
  }
}
