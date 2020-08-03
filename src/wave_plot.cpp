#include <limits>
#include "wave_plot.h"

double average(const int16_t values[], const size_t size) {
  double avg = 0;
  size_t t = 1;

  for (size_t i = 0; i < size; ++i) {
    avg += (values[i] - avg) / t;
    ++t;
  }

  return avg;
}

bool wave_plot(const WaveFile& wave, std::ofstream& ofs) {
  // 8-bit samples are stored as unsigned bytes, ranging from 0 to 255
  // 16-bit samples are stored as 2's-complement signed integers,
  //    ranging from -32768 to 32767
  auto max_amplitude = std::numeric_limits<int16_t>::max();

  // ratio how much original data should be shrinked
  size_t h_ratio = 1000;
  size_t v_ratio = 1000;

  // divide by 2 because two channels
  size_t image_width = wave.data_size / 2 / h_ratio;
  size_t image_height = max_amplitude / v_ratio;

  unsigned char image[image_height][image_width * 3];

  int16_t column[h_ratio];
  size_t column_index = 0;
  size_t col = 0;

  for (size_t i = 0; i < image_width * h_ratio * 2; ++i) {
    // left channel
    if (i % 2 == 0) {
      column[column_index] = wave.data[i];
      column_index++;

      if (column_index == h_ratio) {
        auto y = average(column, h_ratio);

        auto top = (y / static_cast<uint16_t>(v_ratio * 2)) + image_height / 2;
        auto bottom = top + image_height / 2;

        for (size_t row = 0; row < image_height; ++row) {
          if (row < top || row > bottom) {
            image[row][col * 3] = 255;
            image[row][col * 3 + 1] = 255;
            image[row][col * 3 + 2] = 255;
          } else {
            image[row][col * 3] = 255;
            image[row][col * 3 + 1] = 0;
            image[row][col * 3 + 2] = 0;
          }
        }

        col++;
        column_index = 0;
      }
    }
  }

  ofs << "P6\n";
  ofs << image_width << " " << image_height << "\n";
  ofs << "255\n";

  for (size_t row = 0; row < image_height; ++row) {
    for (size_t col = 0; col < image_width; ++col) {
      ofs << image[row][col * 3];
      ofs << image[row][col * 3 + 1];
      ofs << image[row][col * 3 + 2];
    }
  }

  return true;
}
