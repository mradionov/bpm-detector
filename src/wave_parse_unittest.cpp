#include <vector>

#include "../extern/catch/catch2.hpp"

#include "wave_file.h"
#include "wave_parse.h"

TEST_CASE("empty data") {
  std::vector<unsigned char> data = {};
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("does not start with riff") {
  std::vector<unsigned char> data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("chunk size does not match left data size") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x04, 0x00, 0x00, 0x00, // chunk size
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
  };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("incorrect format") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x04, 0x00, 0x00, 0x00, // chunk size
    0x46, 0x4f, 0x4f, 0x4f, // "FOOO"
  };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("incorrect fmt id") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x08, 0x00, 0x00, 0x00, // chunk size
    0x57, 0x41, 0x56, 0x45, // "WAVE"
    0x46, 0x4f, 0x4f, 0x4f, // "FOOO"
  };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("unsupported audio format") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x0e, 0x00, 0x00, 0x00, // chunk size
    0x57, 0x41, 0x56, 0x45, // "WAVE"
    0x66, 0x6d, 0x74, 0x20, // "fmt "
    0x02, 0x00, 0x00, 0x00, // subchunk1 size
    0x02, 0x00,             // unsupported audio format
  };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("unsupported number of channels") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x10, 0x00, 0x00, 0x00, // chunk size
    0x57, 0x41, 0x56, 0x45, // "WAVE"
    0x66, 0x6d, 0x74, 0x20, // "fmt "
    0x02, 0x00, 0x00, 0x00, // subchunk1 size
    0x01, 0x00,             // audio format
    0x03, 0x00,             // num channels
  };
  WaveFile wave;

  REQUIRE_THROWS_AS(wave_parse(wave, data), wave_parse_error);
}

TEST_CASE("correct data") {
  std::vector<unsigned char> data = {
    0x52, 0x49, 0x46, 0x46, // "RIFF"
    0x2c, 0x00, 0x00, 0x00, // chunk size
    0x57, 0x41, 0x56, 0x45, // "WAVE"
    0x66, 0x6d, 0x74, 0x20, // "fmt "
    0x10, 0x00, 0x00, 0x00, // subchunk1 size
    0x01, 0x00,             // audio format
    0x02, 0x00,             // num channels
    0x44, 0xac, 0x00, 0x00, // sample rate
    0x10, 0xb1, 0x02, 0x00, // byte rate
    0x04, 0x00,             // block align
    0x10, 0x00,             // bits per sample
    0x64, 0x61, 0x74, 0x61, // "data"
    0x08, 0x00, 0x00, 0x00, // samples size
    0x01, 0x02, 0x03, 0x04, // samples data
    0x05, 0x06, 0x07, 0x08, // ditto
  };
  WaveFile wave;

  wave_parse(wave, data);

  REQUIRE(wave.identifier == "RIFF");
  REQUIRE(wave.chunk_size == 44);
  REQUIRE(wave.format == "WAVE");
  REQUIRE(wave.audio_format == 1);
  REQUIRE(wave.num_channels == 2);
  REQUIRE(wave.sample_rate == 44100);
  REQUIRE(wave.byte_rate == 176400);
  REQUIRE(wave.block_align == 4);
  REQUIRE(wave.bits_per_sample == 16);
  REQUIRE(wave.samples_size == 4);
  REQUIRE(wave.samples[0] == 513);
  REQUIRE(wave.samples[1] == 1027);
  REQUIRE(wave.samples[2] == 1541);
  REQUIRE(wave.samples[3] == 2055);
}
