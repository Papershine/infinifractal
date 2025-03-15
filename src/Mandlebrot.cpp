#include <Mandlebrot.hpp>
#include <numbers>

long double MAX_MODULUS = 2.0L;
int MAX_ITERATIONS = 500;

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
  
  double angle = std::fmod((scaledIterations / 5.0), 360.0);
  double radians = angle * std::numbers::pi / 180.0;

  double offset = 170.0;
  double amplitude = 255.0 - offset - 35.0;
  
  uint8_t r = static_cast<uint8_t>(std::sin(radians) * amplitude + offset);
  uint8_t g = static_cast<uint8_t>(std::sin(radians + 2.0 * std::numbers::pi / 3.0) * amplitude + offset);
  uint8_t b = static_cast<uint8_t>(std::sin(radians + 4.0 * std::numbers::pi / 3.0) * amplitude + offset);
  
  return (r << 16) | (g << 8) | b;
}