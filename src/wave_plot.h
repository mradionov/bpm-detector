#ifndef WAVE_PLOT_H
#define WAVE_PLOT_H

#include <fstream>
#include "wave_file.h"

bool wave_plot(const WaveFile&, std::ofstream&);

#endif
