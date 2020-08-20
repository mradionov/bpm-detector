#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include "bytestream_reader.h"
#include "wave_parse.h"

const char* kIdentifier = "RIFF";
const char* kFormat = "WAVE";
const char* kFmtId = "fmt ";
const char* kDataId = "data";
const uint16_t kAudioFormatPcm = 1;

const char* kUnexpectedEndMsg = "Unexpected end of data";

// http://soundfile.sapp.org/doc/WaveFormat/
void wave_parse(WaveFile& wave, const std::vector<unsigned char>& data) {
  if (data.size() == 0) {
    throw wave_parse_error("Empty data");
  }

  WaveFile tmp;

  BytestreamReader reader(data);

  if (!reader.read_str(tmp.identifier, 4)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (tmp.identifier != kIdentifier) {
    std::stringstream message;
    message << "Identifier '" << kIdentifier << "' not found";
    throw wave_parse_error(message.str());
  }

  if (!reader.read_u32(tmp.chunk_size)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  // "chunk_size" is the size of the entire file in bytes minus 8 bytes for the
  // two fields not included in this count:
  //  "identifier" and "chunk_size" itself.
  if (tmp.chunk_size != reader.left()) {
    throw wave_parse_error("Chunk size does not match file size");
  }

  if (!reader.read_str(tmp.format, 4)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (tmp.format != kFormat) {
    std::stringstream message;
    message << "Format '" << kFormat << "' not found";
    throw wave_parse_error(message.str());
  }

  // fmt chunk

  std::string fmt_id;
  if (!reader.read_str(fmt_id, 4)) {
   throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (fmt_id != kFmtId) {
    std::stringstream message;
    message << "Subchunk ID '" << kFmtId << "' not found";
    throw wave_parse_error(message.str());
  }

  uint32_t subchunk1_size;
  if (!reader.read_u32(subchunk1_size)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  const auto subchunk1_end_pos = static_cast<uint32_t>(reader.pos())
    + subchunk1_size;

  if (!reader.read_u16(tmp.audio_format)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }
  if (tmp.audio_format != kAudioFormatPcm) {
    throw wave_parse_error("Unsupported audio format");
  }

  if (!reader.read_u16(tmp.num_channels)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }
  if (tmp.num_channels > 2) {
    throw wave_parse_error("Unsupported number of channels");
  }

  if (!reader.read_u32(tmp.sample_rate)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (!reader.read_u32(tmp.byte_rate)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (!reader.read_u16(tmp.block_align)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (!reader.read_u16(tmp.bits_per_sample)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }
  if (tmp.bits_per_sample != 16) {
    throw wave_parse_error("Unsupported bits per sample value");
  }

  if (!reader.seek(subchunk1_end_pos)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  // data chunk

  std::string data_id;
  if (!reader.read_str(data_id, 4)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  if (data_id != kDataId) {
    std::stringstream message;
    message << "Data ID '" << kDataId << "' not found";
    throw wave_parse_error(message.str());
  }

  uint32_t samples_size;
  if (!reader.read_u32(samples_size)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  // int16_t consists of two chars, means that the int16_t[] is two times
  // smaller.
  const auto i_samples_size = samples_size / 2;

  auto samples = std::make_unique<unsigned char[]>(samples_size);
  if (!reader.read_bytes(samples.get(), samples_size)) {
    throw wave_parse_error(kUnexpectedEndMsg);
  }

  auto i_samples = std::make_unique<int16_t[]>(i_samples_size);
  std::memcpy(i_samples.get(), samples.get(), samples_size);

  tmp.samples_size = i_samples_size;
  tmp.samples = std::move(i_samples);

  wave = std::move(tmp);
}
