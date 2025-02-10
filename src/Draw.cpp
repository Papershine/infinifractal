#include <Draw.hpp>

void draw(SDL_Surface* surface)
{
  for (int x=0; x < surface->w; x++) {
    for (int y=0; y < surface->h; y++) {
      putPixel(surface, x, y, calculateColor(coordsToComplex(x, y, surface->w, surface->h)));
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

Complex coordsToComplex(int x, int y, int width, int height) 
{
  double REAL_LOWER_BOUND = -2.75;
  double REAL_UPPER_BOUND = 1.75;
  double COMPLEX_LOWER_BOUND = -1.25;
  double COMPLEX_UPPER_BOUND = 1.25;

  long double dReal = (REAL_UPPER_BOUND - REAL_LOWER_BOUND) / static_cast<long double>(width);
  long double dIm = (COMPLEX_UPPER_BOUND - COMPLEX_LOWER_BOUND) / static_cast<long double>(height);

  long double re = REAL_LOWER_BOUND + dReal * x;
  long double im = (COMPLEX_LOWER_BOUND + dIm * y);
  Complex coord(re, im);
  return coord;
}