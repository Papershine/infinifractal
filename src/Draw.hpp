#pragma once

#include <complex>
#include <SDL2/SDL.h>
#include <Mandlebrot.hpp>
#include <Threadpool.hpp>

using Complex = std::complex<long double>;

extern long double REAL_LOWER_BOUND;
extern long double REAL_UPPER_BOUND;
extern long double COMPLEX_LOWER_BOUND;
extern long double COMPLEX_UPPER_BOUND;

void draw(SDL_Surface* surface);
void draw_interruptible(SDL_Surface* surface, SDL_Window* window, std::atomic<bool>* background_draw_allowed);
void putPixel(SDL_Surface* surface, int x, int y, Uint32 color);
Complex coordsToComplex(int x, int y, int width, int height);

void zoom(int mouseX, int mouseY, float zoomFactor, int width, int height);
void resetView();
void pan(int deltaX, int deltaY, int width, int height);
