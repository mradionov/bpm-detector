#include <fstream>
#include <iostream>
#include <string>

// Unpacks little-endian to an integer.
// Note: casting to "unsigned char" because std::ifstream reads "char" by
// default, but in file there might be some sequences that don't fit into
// a "char" (value >127), therefore it overflows (becomes negative).
// Here we are converting those negative values to unsigned char.
// Maybe it can be fixed by providing a type to basic_ifstream<char_type>,
// but not sure which one.

uint16_t le_ch_to_ui16(char* ch) {
  return static_cast<unsigned char>(ch[0])
    | static_cast<unsigned char>(ch[1]) << 8;
}

uint32_t le_ch_to_ui32(char* ch) {
  return static_cast<unsigned char>(ch[0])
    | static_cast<unsigned char>(ch[1]) << 8
    | static_cast<unsigned char>(ch[2]) << 16
    | static_cast<unsigned char>(ch[3]) << 24;
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

  auto chunk_size_i = static_cast<std::uintmax_t>(le_ch_to_ui32(chunk_size));

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
  std::cout << "ChunkSize: " << chunk_size_i << "\n";
  std::cout << "Format: " << format << "\n";
  std::cout << "Subchunk1ID: " << subchunk1_id << "\n";
  std::cout << "Subchunk1Size: " << le_ch_to_ui32(subchunk1_size) << "\n";
  std::cout << "AudioFormat: " << le_ch_to_ui16(audio_format) << "\n";
  std::cout << "NumChannels: " << le_ch_to_ui16(num_channels) << "\n";
  std::cout << "SampleRate: " << le_ch_to_ui32(sample_rate) << "\n";
  std::cout << "ByteRange: " << le_ch_to_ui32(byte_range) << "\n";

  return 0;
}
