#include <Hsl.hpp>

// simple HSL to RGB function adapted from Wikipedia
// https://en.wikipedia.org/wiki/HSL_and_HSV#Color_conversion_formulae
Uint32 Hsl::toRgb() 
{
  double a = s * std::min(this->l, 1 - this->l);
  uint8_t r = this->toRgbHelper(0, a);
  uint8_t g = this->toRgbHelper(8, a);
  uint8_t b = this->toRgbHelper(4, a);
  return (r << 16) | (g << 8) | b;
}

uint8_t Hsl::toRgbHelper(int n, double a) 
{
  double hue = fmod(this->h, 360.0);
  if (hue < 0) hue += 360.0;
  
  int k = static_cast<int>((n + hue / 30.0)) % 12;
  double modifier = this->l - a * std::max(std::min({k-3, 9-k, 1}), -1);
  return static_cast<uint8_t>(255 * modifier);
}
