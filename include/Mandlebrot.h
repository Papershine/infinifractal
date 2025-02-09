#pragma once

#include <sdl2/SDL.h>
#include <complex>

using Complex = std::complex<long double>;

int calculateIterations(Complex z);
Uint32 calculateColor(Complex z);
