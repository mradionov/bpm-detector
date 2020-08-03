#include <cassert>
#include <cstring>
#include "wave_parse.h"

// Used to read multi-byte integers from a WAVE file.
// Copies little-endian char array (extracted from binary WAVE file)
// to little-endian type (most-likely integer).
// Similar to https://en.cppreference.com/w/cpp/numeric/bit_cast
template <typename T>
T ch_to_i(
  const char* ch,
  std::enable_if_t<std::is_trivially_copyable<T>::value, int> = 0
) {
  static_assert(
    std::is_trivially_constructible<T>::value,
    "Provided int has to be trivially constructible"
  );

  // TODO: Why fails?
  // assert(sizeof(ch) == sizeof(T));

  T val;
  std::memcpy(&val, ch, sizeof(T));
  return val;
}

std::ifstream& read_str(
  std::ifstream& file,
  std::string& str,
  const std::streamsize size = 4
) {
  // TODO: can read right intro string?
  char ch[size + 1];
  ch[size] = '\0';

  file.read(ch, size);

  str = std::string(ch);

  return file;
}

template <typename T>
std::ifstream& read_num(std::ifstream& file, T& num) {
  constexpr auto size = sizeof(T);

  char ch[size];

  file.read(ch, size);

  num = ch_to_i<T>(ch);

  return file;
}

std::ifstream& skip(std::ifstream& file, const std::streamsize size) {
  file.ignore(size);
  return file;
}

// http://soundfile.sapp.org/doc/WaveFormat/
bool wave_parse(WaveFile& wave, std::ifstream& file) {
  if (!file) {
    return false;
  }

  // Main chunk

  read_str(file, wave.identifier);
  // Skip chunk size
  skip(file, 4);
  read_str(file, wave.format);

  // fmt chunk

  // Skip chunk name "fmt" and size
  skip(file, 8);

  read_num(file, wave.audio_format);
  read_num(file, wave.num_channels);
  read_num(file, wave.sample_rate);
  read_num(file, wave.byte_range);
  read_num(file, wave.block_align);
  read_num(file, wave.bits_per_sample);

  // data chunk

  // Skip chunk name "data"
  skip(file, 4);

  uint32_t data_size_bytes;

  read_num(file, data_size_bytes);

  char data[data_size_bytes];
  file.read(data, data_size_bytes);

  // int16_t consists of two chars, means that the int16_t[] is two times
  // smaller.

  wave.data_size = data_size_bytes / 2;

  // bit cast chars to int16_t
  // TODO: looks shit because a lot of copy (file -> char -> int16_t)
  int16_t i_data[wave.data_size];
  std::memcpy(i_data, data, sizeof(i_data));
  wave.data = i_data;

  return true;
}
