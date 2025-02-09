#include "Draw.h"
#include "Mandlebrot.h"

void draw(SDL_Surface* surface, int width, int height)
{
  for (unsigned x=0; x<width; x++) {
    for (unsigned y=0; y<height; y++) {
      // Calculate the color at this point
      // Set the pixel to that color
      putPixel(surface, x, y, calculateColor(coordsToComplex(x, y, width, height)));
    }
  }
}

void putPixel(SDL_Surface* surface, int x, int y, Uint32 color) 
{
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32* pixels = (Uint32*)surface->pixels;
        pixels[(y * surface->w) + x] = color;
    }
}

Uint32 coordsToRGB(int x, int y) {
  uint8_t r = (x * 37) % 256;  // Generate Red component
  uint8_t g = (y * 53) % 256;  // Generate Green component
  uint8_t b = ((x + y) * 71) % 256;  // Generate Blue component

  return (r << 16) | (g << 8) | b;
}

Complex coordsToComplex(int x, int y, int width, int height) {
  int REAL_LOWER_BOUND = -2;
  int REAL_UPPER_BOUND = 2;
  int COMPLEX_LOWER_BOUND = -1;
  int COMPLEX_UPPER_BOUND = 1;

  long double dReal = (REAL_UPPER_BOUND - REAL_LOWER_BOUND) / static_cast<long double>(width);
  long double dIm = (COMPLEX_UPPER_BOUND - COMPLEX_LOWER_BOUND) / static_cast<long double>(height);

  long double re = REAL_LOWER_BOUND + dReal * x;
  long double im = (COMPLEX_LOWER_BOUND + dIm * y);
  Complex coord(re, im);
  return coord;
}