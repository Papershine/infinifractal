#pragma once

#include <SDL2/SDL.h>
#include <complex>

using Complex = std::complex<long double>;

void draw(SDL_Surface* surface);
void putPixel(SDL_Surface* surface, int x, int y, Uint32 color);
Uint32 coordsToRGB(int x, int y);
Complex coordsToComplex(int x, int y, int width, int height);
