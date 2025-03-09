#include <Mandlebrot.hpp>
#include <cmath>

long double MAX_MODULUS = 2.0L;
int MAX_ITERATIONS = 1000;

int calculateIterations(Complex z)
{
  Complex value(0.0L, 0.0L);
  int iterations = 0;
  
  long double modulusSquared = 0.0L;
  
  while (modulusSquared <= MAX_MODULUS * MAX_MODULUS && iterations < MAX_ITERATIONS) {
    value = Complex(
      value.real() * value.real() - value.imag() * value.imag(),
      2.0L * value.real() * value.imag()
    ) + z;
    
    modulusSquared = value.real() * value.real() + value.imag() * value.imag();
    iterations++;
  }
  
  if (iterations < MAX_ITERATIONS) {
    long double smooth = iterations + 1.0L - std::log(std::log(std::sqrt(modulusSquared))) / std::log(2.0L);
    return static_cast<int>(smooth * 100);
  }
  
  return iterations * 100;
}

Uint32 calculateColor(Complex z)
{
  int scaledIterations = calculateIterations(z);
  
  if (scaledIterations >= MAX_ITERATIONS * 100) {
    return (100 << 16) | (100 << 8) | 100;
  }
  
  double hue = fmod((scaledIterations / 2.0), 360.0);
  
  double saturation = 0.4;
  double lightness = 0.8;
  
  return Hsl(hue, saturation, lightness).toRgb();
}