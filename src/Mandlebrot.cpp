#include <Mandlebrot.hpp>
#include <Color.hpp>
#include <numbers>
#include <iostream>

constexpr double MAX_MODULUS = 2.0;

#ifdef __EMSCRIPTEN__
constexpr uint16_t MAX_ITERATIONS = 100;
#else
constexpr uint16_t MAX_ITERATIONS = 500;
#endif

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

  int i = (scaledIterations + LUT_SIZE/4) % LUT_SIZE;
  uint8_t r = getColorLUT().r[i];
  uint8_t g = getColorLUT().g[i];
  uint8_t b = getColorLUT().b[i];
  
  return (r << 16) | (g << 8) | b;
}