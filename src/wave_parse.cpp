#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include "wave_parse.h"

const char* kIdentifier = "RIFF";
const char* kFormat = "WAVE";
const char* kFmtId = "fmt ";
const char* kDataId = "data";

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

std::ifstream& peek_str(
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
void wave_parse(WaveFile& wave, std::ifstream& file) {
  // Main chunk

  WaveFile tmp;

  read_str(file, tmp.identifier);

  if (tmp.identifier != kIdentifier) {
    std::stringstream message;
    message << "Identifier '" << kIdentifier << "' not found";
    throw wave_parse_error(message.str());
  }

  read_num(file, tmp.chunk_size);
  read_str(file, tmp.format);

  if (tmp.format != kFormat) {
    std::stringstream message;
    message << "Format '" << kFormat << "' not found";
    throw wave_parse_error(message.str());
  }

  // fmt chunk

  std::string fmt_id;
  read_str(file, fmt_id);

  if (fmt_id != kFmtId) {
    std::stringstream message;
    message << "Subchunk '" << kFmtId << "' ID not found";
    throw wave_parse_error(message.str());
  }

  uint32_t subchunk1_size;
  read_num(file, subchunk1_size);

  auto subchunk1_end_pos = static_cast<uint32_t>(file.tellg())
    + subchunk1_size;

  read_num(file, tmp.audio_format);
  read_num(file, tmp.num_channels);
  read_num(file, tmp.sample_rate);
  read_num(file, tmp.byte_range);
  read_num(file, tmp.block_align);
  read_num(file, tmp.bits_per_sample);

  file.seekg(subchunk1_end_pos);


  // data chunk

  std::string data_id;
  read_str(file, data_id);

  if (data_id != kDataId) {
    std::stringstream message;
    message << "Data '" << kDataId << "' ID not found";
    throw wave_parse_error(message.str());
  }

  uint32_t data_size;
  read_num(file, data_size);

  auto data = std::make_unique<char[]>(data_size);
  file.read(data.get(), data_size);

  // int16_t consists of two chars, means that the int16_t[] is two times
  // smaller.
  auto i_data_size = data_size / 2;
  auto i_data = std::make_unique<int16_t[]>(i_data_size);
  std::memcpy(i_data.get(), data.get(), data_size);

  tmp.data_size = i_data_size;
  tmp.data = std::move(i_data);

  wave = std::move(tmp);
}
