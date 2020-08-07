#ifndef WAVE_PLOT_H
#define WAVE_PLOT_H

#include <fstream>
#include <string>
#include "wave_file.h"

class wave_plot_error : public std::runtime_error {
public:
  wave_plot_error(const char* what_arg): std::runtime_error(what_arg) {}
  wave_plot_error(std::string what_arg): std::runtime_error(what_arg) {}
};

void wave_plot_amplitude(const WaveFile&, std::string);

#endif
