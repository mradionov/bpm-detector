#ifndef WAVE_FILE_H
#define WAVE_FILE_H

#include <cstdint>
#include <memory>
#include <string>

struct WaveFile {
  std::string identifier;
  uint32_t chunk_size;
  std::string format;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_range;
  uint16_t block_align;
  uint16_t bits_per_sample;

  // Number of bytes in data
  uint32_t data_size;

  // 8-bit samples are stored as unsigned bytes, ranging from 0 to 255
  // 16-bit samples are stored as 2's-complement signed integers,
  //    ranging from -32768 to 32767
  std::unique_ptr<int16_t[]> data;
};

#endif
