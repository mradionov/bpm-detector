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
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;

  // Number of bytes in data
  uint32_t samples_size;

  // 8-bit samples are stored as unsigned bytes, ranging from 0 to 255
  // 16-bit samples are stored as 2's-complement signed integers,
  //    ranging from -32768 to 32767
  std::unique_ptr<int16_t[]> samples;

  WaveFile():
    chunk_size(0),
    audio_format(0),
    num_channels(0),
    sample_rate(0),
    byte_rate(0),
    block_align(0),
    bits_per_sample(0),
    samples_size(0)
  {}
};

#endif
