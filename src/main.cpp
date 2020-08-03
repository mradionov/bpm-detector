#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include "wave_file.h"
#include "wave_parse.h"
#include "wave_plot.h"

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

  std::cout << "sample 1: " << wave.data[0] << "\n";
  std::cout << "sample 2: " << wave.data[1] << "\n";
  std::cout << "sample 3: " << wave.data[2] << "\n";
  std::cout << "sample 4: " << wave.data[4] << "\n";

  std::ofstream image_file(
    "../test/data/image.ppm",
    std::ios::trunc
  );

  if (!image_file) {
    std::cerr << "Cant open image file" << std::endl;
    return 1;
  }

  wave_plot(wave, image_file);

  return 0;
}
