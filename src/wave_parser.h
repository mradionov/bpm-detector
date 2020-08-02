#ifndef WAVE_PARSER_H
#define WAVE_PARSER_H

#include <fstream>

struct WaveFile {
  std::string identifier;
  std::string format;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_range;
  uint16_t block_align;
  uint16_t bits_per_sample;
  uint32_t data_size;

};

bool wave_parse(WaveFile&, std::ifstream&);

#endif
