#include "ppm_image.h"

PpmPixel::PpmPixel(): r(0), g(0), b(0) {}

PpmPixel::PpmPixel(const PpmColor vr, const PpmColor vg, const PpmColor vb) {
  set(vr, vg, vb);
}

void PpmPixel::set(const PpmColor vr, const PpmColor vg, const PpmColor vb) {
  r = vr;
  g = vg;
  b = vb;
}

void PpmPixel::set(const PpmPixel other) {
  r = other.r;
  g = other.g;
  b = other.b;
}

PpmImage::PpmImage(const size_t width, const size_t height):
  m_width(width),
  m_height(height),
  m_pixels(width * height),
  m_color(0, 0, 0)
{}

size_t PpmImage::width() const {
  return m_width;
}

size_t PpmImage::height() const {
  return m_height;
}

void PpmImage::set_color(
  const PpmColor r, const PpmColor g, const PpmColor b
) {
  m_color.set(r, g, b);
}

void PpmImage::set_color(const PpmPixel c) {
  m_color = c;
}

void PpmImage::draw_pixel(const size_t x, const size_t y) {
  const auto i = index(x, y);
  m_pixels[i].set(m_color);
}

const PpmPixel PpmImage::get_pixel(const size_t x, const size_t y) const {
  const auto i = index(x, y);
  return m_pixels[i];
}

size_t PpmImage::index(const size_t x, const size_t y) const {
  return y * m_width + x;
}

std::ofstream& operator<<(std::ofstream& ofs, const PpmImage& image) {
  auto width = image.width();
  auto height = image.height();

  ofs << "P6\n";
  ofs << width << " " << height << "\n";
  ofs << "255\n";

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      const auto pixel = image.get_pixel(x, y);

      ofs << pixel.r << pixel.g << pixel.b;
    }
  }

  return ofs;
}
