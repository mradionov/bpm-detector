#include <complex>
#include <fstream>
#include <iostream>
#include <vector>
#include "fourier_transform.h"
#include "wave_file.h"
#include "wave_parse.h"
#include "wave_plot.h"

int main() {
  const char* audio_filename = "../test/data/test6.wav";

  std::ifstream file(audio_filename, std::ios::binary);

  if (!file) {
    std::cerr << "Cant open file '" << audio_filename << "'" << std::endl;
    return 1;
  }

  std::vector<unsigned char> file_buffer(
    (std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>()
  );

  std::cout << "Parsing file '" << audio_filename << "'... ";

  WaveFile wave;

  try {
    wave_parse(wave, file_buffer);
  } catch (wave_parse_error err) {
    std::cout << "Error.\nWave parse error: " << err.what() << std::endl;
    return 1;
  }

  std::cout << "Done.\n";
  std::cout << std::endl;

  std::cout << "Identifier: " << wave.identifier << "\n";
  std::cout << "Format: " << wave.format << "\n";
  std::cout << "AudioFormat: " << wave.audio_format << "\n";
  std::cout << "NumChannels: " << wave.num_channels << "\n";
  std::cout << "SampleRate: " << wave.sample_rate << "\n";
  std::cout << "ByteRate: " << wave.byte_rate << "\n";
  std::cout << "BlockAlign: " << wave.block_align << "\n";
  std::cout << "BitsPerSample: " << wave.bits_per_sample << "\n";
  std::cout << "SampleSize: " << wave.samples_size << "\n";

  for (size_t i = 0; i < 10; i++) {
    std::cout << "sample " << i << ": " << wave.samples[i] << "\n";
  }
  std::cout << "\n";

  const char* plot_filename = "../test/data/image.ppm";
  std::ofstream image_file(plot_filename, std::ios::trunc);
  if (!image_file) {
    std::cout << "Main: can't open plot file" << std::endl;
    return 1;
  }

  image_file << wave_plot_amplitude(wave);

  return 0;
}
