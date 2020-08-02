#include <cassert>
#include <cstring>
#include "wave_parser.h"

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
  // sizeof inline to init array at compile-time
  // otherwise "ch_to_i" template would not know the size of the array
  char ch[sizeof(T)];

  file.read(ch, sizeof(T));

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
  read_num(file, wave.data_size);

  // skip(

  // char format[5];
  // format[4] = '\0';
  // file.read(format, 4);
  // file.get(wave.format, 4);

  return true;
}
