#include "Mandlebrot.h"

int MANDLEBROT_THRESHOLD = 1000;
int MAX_ITERATIONS = 50;

int calculateIterations(Complex z)
{
  Complex value = 0;
  int iterations = 0;
  while (std::abs(value) < MANDLEBROT_THRESHOLD && iterations < MAX_ITERATIONS) {
    value = std::pow(value, 2) + z;
    iterations++;
  }
  return iterations;
}

Uint32 calculateColor(Complex z)
{
  int it = calculateIterations(z);
  uint8_t color = static_cast<uint8_t>(255 * (static_cast<double>(it) / MAX_ITERATIONS));
  return (color << 16) | (color << 8) | color;
}