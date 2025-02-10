#pragma once

#include <complex>
#include <SDL2/SDL.h>
#include <Mandlebrot.hpp>

using Complex = std::complex<long double>;

void draw(SDL_Surface* surface);
void putPixel(SDL_Surface* surface, int x, int y, Uint32 color);
Complex coordsToComplex(int x, int y, int width, int height);
