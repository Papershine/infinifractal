#include <Mandlebrot.hpp>
#include <Color.hpp>
#include <numbers>
#include <iostream>

constexpr double MAX_MODULUS = 2.0;
constexpr double MAX_MODULUS_SQUARED = MAX_MODULUS * MAX_MODULUS;

#ifdef __EMSCRIPTEN__
constexpr uint16_t MAX_ITERATIONS = 250;
#else
constexpr uint16_t MAX_ITERATIONS = 500;
#endif

int calculateIterations(Complex c)
{
  Complex z(0.0L, 0.0L);
  uint16_t iterations = 0;
  
  float modulusSquared = 0.0f;

  long double nextReal = 0.0L;
  long double nextImag = 0.0L;
  
  while (iterations < MAX_ITERATIONS && modulusSquared <= MAX_MODULUS_SQUARED) {
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

Uint32 calculateColor(Complex z, const SDL_PixelFormat *format)
{
  int scaledIterations = calculateIterations(z);
  
  if (scaledIterations >= MAX_ITERATIONS * 100) {
    return (100 << 16) | (100 << 8) | 100;
  }

  int i = (scaledIterations + LUT_SIZE/4) % LUT_SIZE;
  uint8_t r = get_color_lut().r[i];
  uint8_t g = get_color_lut().g[i];
  uint8_t b = get_color_lut().b[i];
  
  return SDL_MapRGB(format, r, g, b);;
}