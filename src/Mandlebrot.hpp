#pragma once

#include <algorithm>
#include <complex>
#include <SDL2/SDL.h>

using Complex = std::complex<long double>;

int calculateIterations(Complex z);
Uint32 calculateColor(Complex z, const SDL_PixelFormat *format);
