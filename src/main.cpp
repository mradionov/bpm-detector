#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <cmath>
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

  std::cout << "Parsing file '" << audio_filename << "'... ";

  WaveFile wave;

  try {
    wave_parse(wave, file);
  } catch (wave_parse_error err) {
    std::cout << "Wave parse error: " << err.what() << std::endl;
    return 1;
  }

  std::cout << "Done.\n";

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
  std::cout << "sample 4: " << wave.data[3] << "\n";

  try {
    wave_plot_amplitude(wave, "../test/data/image.ppm");
  } catch (wave_plot_error err) {
    std::cout << "Wave plot error: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}
