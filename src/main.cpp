#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

template <typename T>
T ch_to_i(const char* ch) {
  T val;
  std::memcpy(&val, ch, sizeof(T));
  return val;
}

int main() {
  const char* filename = "../test/data/test1.wav";

  std::ifstream file(filename, std::ios::binary);

  if (!file) {
    std::cerr << "Cant open file '" << filename << "'" << std::endl;
    return 1;
  }

  char chunk_id[5];
  chunk_id[4] = '\0';
  file.read(chunk_id, 4);

  char chunk_size[4];
  file.read(chunk_size, 4);

  char format[5];
  format[4] = '\0';
  file.read(format, 4);

  char subchunk1_id[5];
  subchunk1_id[4] = '\0';
  file.read(subchunk1_id, 4);

  char subchunk1_size[4];
  file.read(subchunk1_size, 4);

  char audio_format[2];
  file.read(audio_format, 2);

  char num_channels[2];
  file.read(num_channels, 2);

  char sample_rate[4];
  file.read(sample_rate, 4);

  char byte_range[4];
  file.read(byte_range, 4);

  std::cout << "ChunkID: " << chunk_id << "\n";
  std::cout << "ChunkSize: " << ch_to_i<uint32_t>(chunk_size) << "\n";
  std::cout << "Format: " << format << "\n";
  std::cout << "Subchunk1ID: " << subchunk1_id << "\n";
  std::cout << "Subchunk1Size: " << ch_to_i<uint32_t>(subchunk1_size) << "\n";
  std::cout << "AudioFormat: " << ch_to_i<uint16_t>(audio_format) << "\n";
  std::cout << "NumChannels: " << ch_to_i<uint16_t>(num_channels) << "\n";
  std::cout << "SampleRate: " << ch_to_i<uint32_t>(sample_rate) << "\n";
  std::cout << "ByteRange: " << ch_to_i<uint32_t>(byte_range) << "\n";

  return 0;
}
