#ifndef WAVE_PARSE_H
#define WAVE_PARSE_H

#include <fstream>
#include "wave_file.h"

bool wave_parse(WaveFile&, std::ifstream&);

#endif
