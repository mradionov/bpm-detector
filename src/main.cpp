#include <fstream>
#include <iostream>

#include <string>
#include <limits>
#include "wave_parser.h"

double average(const int16_t values[], const size_t size) {
  double avg = 0;
  size_t t = 1;

  for (size_t i = 0; i < size; ++i) {
    avg += (values[i] - avg) / t;
    ++t;
  }

  return avg;
}

int main() {
  const char* audio_filename = "../test/data/test1.wav";

  std::ifstream file(audio_filename, std::ios::binary);

  if (!file) {
    std::cerr << "Cant open file '" << audio_filename << "'" << std::endl;
    return 1;
  }

  WaveFile wave;

  wave_parse(wave, file);

  std::cout << "Identifier: " << wave.identifier << "\n";
  std::cout << "Format: " << wave.format << "\n";
  std::cout << "AudioFormat: " << wave.audio_format << "\n";
  std::cout << "NumChannels: " << wave.num_channels << "\n";
  std::cout << "SampleRate: " << wave.sample_rate << "\n";
  std::cout << "ByteRange: " << wave.byte_range << "\n";
  std::cout << "BlockAlign: " << wave.block_align << "\n";
  std::cout << "BitsPerSample: " << wave.bits_per_sample << "\n";
  std::cout << "DataSize: " << wave.data_size << "\n";

  // size_t bytes_per_sample = i_bits_per_sample / 8;

  // auto samples_size = static_cast<size_t>(ch_to_i<uint32_t>(subchunk2_size));

  // char samples[samples_size];
  // file.read(samples, samples_size);

  // int16_t i_samples[samples_size / 2];
  // std::memcpy(i_samples, samples, sizeof(i_samples));

  // std::cout << "ChunkID: " << chunk_id << "\n";
  // std::cout << "ChunkSize: " << ch_to_i<uint32_t>(chunk_size) << "\n";
  // std::cout << "Format: " << format << "\n";
  // std::cout << "Subchunk1ID: " << subchunk1_id << "\n";
  // std::cout << "Subchunk1Size: " << ch_to_i<uint32_t>(subchunk1_size) << "\n";

  //
  //

  // std::cout << "Subchunk2ID: " << subchunk2_id << "\n";
  // std::cout << "Subchunk2Size: " << ch_to_i<uint32_t>(subchunk2_size) << "\n";

  // // std::cout << "bytes per sample: " << bytes_per_sample << "\n";
  // // std::cout << "sample 1: " << ch_to_i<int16_t>(sample1) << "\n";
  // // std::cout << "sample 2: " << ch_to_i<int16_t>(sample2) << "\n";
  // // std::cout << "sample 1: " << i_samples[0] << "\n";
  // // std::cout << "sample 1: " << i_samples[2] << "\n";
  // // std::cout << "sample 1: " << i_samples[3] << "\n";

  // int16_t max_amp = std::numeric_limits<int16_t>::max();

  // size_t h_ratio = 1000;
  // size_t v_ratio = 1000;

  // size_t image_width = ((samples_size / 2) / 2) / h_ratio;
  // size_t image_height = max_amp / v_ratio;

  // std::cout << "ImageWidth: " << image_width << "\n";
  // std::cout << "ImageHeight: " << image_height << "\n";

  // int16_t column[h_ratio];
  // size_t column_index = 0;

  // // std::fill(column, column + h_ratio, 0);

  // unsigned char image[image_height][image_width * 3];

  // size_t col = 0;

  // for (size_t i = 0; i < image_width * h_ratio * 2; ++i) {
  //   // left channel
  //   if (i % 2 == 0) {
  //     column[column_index] = i_samples[i];
  //     column_index++;

  //     if (column_index == h_ratio) {
  //       int16_t y = average(column, h_ratio);


  //       int16_t top = (y / static_cast<uint16_t>(v_ratio * 2)) + image_height / 2;
  //       int16_t bottom = top + image_height / 2;

  //       std::cout << top << " - " << bottom << std::endl;
  //       // for (size_t k = 0; k < h_ratio; ++k) {
  //       //   std::cout << column[k] << ", ";
  //       // }

  //       // std::cout << std::endl;

  //       for (size_t row = 0; row < image_height; ++row) {


  //         if (row < top || row > bottom) {
  //           image[row][col * 3] = 255;
  //           image[row][col * 3 + 1] = 255;
  //           image[row][col * 3 + 2] = 255;
  //         } else {
  //           image[row][col * 3] = 255;
  //           image[row][col * 3 + 1] = 0;
  //           image[row][col * 3 + 2] = 0;
  //         }
  //       }

  //       col++;
  //       column_index = 0;
  //       // std::fill(column, column + h_ratio, 0);
  //     }
  //   }
  // }

  // std::cout << column_index << std::endl;
  // std::cout << col << std::endl;

  // // if (column_index != 0) {
  // //   size_t y = average(column, h_ratio);

  // //   for (size_t j = 0; j < image_height; ++j) {
  // //     image[j][image_x * 3] = 255;
  // //     image[j][image_x * 3 + 1] = 0;
  // //     image[j][image_x * 3 + 2] = 0;
  // //   }
  // // }

  // std::ofstream image_file(
  //   "../test/data/image.ppm",
  //   std::ios::trunc
  // );

  // if (!image_file) {
  //   std::cerr << "Cant open image file" << std::endl;
  //   return 1;
  // }

  // image_file << "P6\n";
  // image_file << image_width << " " << image_height << "\n";
  // image_file << "255\n";

  // for (size_t row = 0; row < image_height; ++row) {
  //   for (size_t col = 0; col < image_width; ++col) {
  //     image_file << image[row][col * 3];
  //     image_file << image[row][col * 3 + 1];
  //     image_file << image[row][col * 3 + 2];
  //     // std::cout << image[row][col * 3] << " ";
  //     // std::cout << image[row][col * 3 + 1] << " ";
  //     // std::cout << image[row][col * 3 + 2] << "| ";
  //     // image_file << image[row][col * 3] << " ";
  //     // image_file << image[row][col * 3 + 1] << " ";
  //     // image_file << image[row][col * 3 + 2] << "\n";
  //     // image_file.put(image[row][col * 3]);
  //     // image_file.put(' ');
  //     // image_file << image[row][col * 3 + 1] << " ";
  //     // image_file << image[row][col * 3 + 2] << "\n";
  //   }
  //   // std::cout << "\n";
  // }

  return 0;
}
