#include <algorithm>
#include "Mandlebrot.h"
#include "Hsl.h"

int MANDLEBROT_THRESHOLD = 100;
int MAX_ITERATIONS = 25;

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
  uint16_t hue = static_cast<uint8_t>(230 * (static_cast<double>(it) / MAX_ITERATIONS));
  
  return Hsl(hue, 0.75, 0.75).toRgb();
}