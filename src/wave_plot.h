#ifndef WAVE_PLOT_H
#define WAVE_PLOT_H

#include <fstream>
#include <string>
#include "ppm_image.h"
#include "wave_file.h"

PpmImage wave_plot_amplitude(const WaveFile&);

#endif
