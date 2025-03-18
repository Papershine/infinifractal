#include <Mandlebrot.hpp>
#include <numbers>

double MAX_MODULUS = 2.0;
uint16_t MAX_ITERATIONS = 400;

int calculateIterations(Complex c)
{
  Complex z(0.0L, 0.0L);
  uint16_t iterations = 0;
  
  float modulusSquared = 0.0f;
  float maxModulusSquared = MAX_MODULUS * MAX_MODULUS;

  long double nextReal = 0.0L;
  long double nextImag = 0.0L;
  
  while (iterations < MAX_ITERATIONS && modulusSquared <= maxModulusSquared) {
    #pragma float_control(precise, off)
    z = Complex(
      nextReal,
      nextImag
    ) + c;
    
    long double real = z.real() * z.real();
    long double imag = z.imag() * z.imag();

    modulusSquared = real + imag;
    iterations++;

    nextReal = real - imag;
    nextImag = 2.0L * z.real() * z.imag();
  }

  if (iterations < MAX_ITERATIONS) {
    #pragma float_control(precise, off)
    float smooth = iterations + 1.0f - std::log(std::log(std::sqrt(modulusSquared))) / std::log(2.0f);
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
  
  double angle = std::fmod((scaledIterations / 10.0 + 150.0), 360.0);
  double radians = angle * std::numbers::pi / 180.0;

  double offset = 170.0;
  double amplitude = 255.0 - offset - 35.0;
  
  uint8_t r = static_cast<uint8_t>(std::sin(radians) * amplitude + offset);
  uint8_t g = static_cast<uint8_t>(std::sin(radians + 2.0 * std::numbers::pi / 3.0) * amplitude + offset);
  uint8_t b = static_cast<uint8_t>(std::sin(radians + 4.0 * std::numbers::pi / 3.0) * amplitude + offset);
  
  return (r << 16) | (g << 8) | b;
}