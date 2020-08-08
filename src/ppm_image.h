#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include <cstddef>
#include <fstream>
#include <vector>

using PpmColor = unsigned char;

struct PpmPixel {
  PpmPixel();
  PpmPixel(const PpmColor, const PpmColor, const PpmColor);
  void set(const PpmColor, const PpmColor, const PpmColor);
  void set(const PpmPixel);
  PpmColor r;
  PpmColor g;
  PpmColor b;
};

class PpmImage {
public:
  PpmImage(const size_t, const size_t);
  size_t width() const;
  size_t height() const;
  void set_color(const PpmColor, const PpmColor, const PpmColor);
  void set_color(const PpmPixel);
  void draw_pixel(const size_t, const size_t);
  void draw_column(const size_t, const size_t, const size_t);
  void draw_row(const size_t, const size_t, const size_t);
  void draw_rect(const size_t, const size_t, const size_t, const size_t);
  const PpmPixel get_pixel(const size_t, const size_t) const;
private:
  size_t m_width;
  size_t m_height;
  std::vector<PpmPixel> m_pixels;
  PpmPixel m_color;
  size_t index(const size_t, const size_t) const;
};

std::ofstream& operator<<(std::ofstream&, const PpmImage&);

#endif
