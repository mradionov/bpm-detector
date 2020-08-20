#ifndef WAVE_PARSE_H
#define WAVE_PARSE_H

#include <string>
#include <vector>
#include "wave_file.h"

class wave_parse_error : public std::runtime_error {
public:
  wave_parse_error(const char* what_arg): std::runtime_error(what_arg) {}
  wave_parse_error(std::string what_arg): std::runtime_error(what_arg) {}
};

void wave_parse(WaveFile&, const std::vector<unsigned char>&);

#endif
