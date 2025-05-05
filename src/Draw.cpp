#include <Draw.hpp>
#include <thread>
#include <iostream>

long double REAL_LOWER_BOUND = -2.75L;
long double REAL_UPPER_BOUND = 1.75L;
long double COMPLEX_LOWER_BOUND = -1.25L;
long double COMPLEX_UPPER_BOUND = 1.25L;

void task(int y, int width, int height, Uint32* buf) {
  std::vector<Uint32> task_buffer;

  for (int x=0; x < width; x++) {
    task_buffer.push_back(calculateColor(coordsToComplex(x, y, width, height)));
  }

  {
    std::copy(task_buffer.begin(), task_buffer.end(), buf + (y * width));
  }
}

void draw(SDL_Surface* surface)
{
  std::vector<Uint32> pixel_buffer(surface->w * surface->h);
  Threadpool pool;

  for (int y=0; y < surface->h; y++) {
    pool.schedule(task, y, surface->w, surface->h, (Uint32*)surface->pixels);
  }

  pool.join();
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
  long double dReal = (REAL_UPPER_BOUND - REAL_LOWER_BOUND) / static_cast<long double>(width);
  long double dIm = (COMPLEX_UPPER_BOUND - COMPLEX_LOWER_BOUND) / static_cast<long double>(height);

  long double re = REAL_LOWER_BOUND + dReal * x;
  long double im = COMPLEX_LOWER_BOUND + dIm * y;
  Complex coord(re, im);
  return coord;
}

void zoom(int mouseX, int mouseY, float zoomFactor, int width, int height) {
  Complex mouseComplex = coordsToComplex(mouseX, mouseY, width, height);
  
  long double realWidth = REAL_UPPER_BOUND - REAL_LOWER_BOUND;
  long double complexHeight = COMPLEX_UPPER_BOUND - COMPLEX_LOWER_BOUND;
  
  long double newRealWidth = realWidth * zoomFactor;
  long double newComplexHeight = complexHeight * zoomFactor;
  
  long double relativeX = (mouseComplex.real() - REAL_LOWER_BOUND) / realWidth;
  long double relativeY = (mouseComplex.imag() - COMPLEX_LOWER_BOUND) / complexHeight;
  
  REAL_LOWER_BOUND = mouseComplex.real() - (relativeX * newRealWidth);
  REAL_UPPER_BOUND = REAL_LOWER_BOUND + newRealWidth;
  
  COMPLEX_LOWER_BOUND = mouseComplex.imag() - (relativeY * newComplexHeight);
  COMPLEX_UPPER_BOUND = COMPLEX_LOWER_BOUND + newComplexHeight;
  
  #ifndef __EMSCRIPTEN__
  std::cout << "New bounds: [" << REAL_LOWER_BOUND << ", " << REAL_UPPER_BOUND << "] x [" 
            << COMPLEX_LOWER_BOUND << ", " << COMPLEX_UPPER_BOUND << "]\n";
  #endif
}

void resetView() {
  REAL_LOWER_BOUND = -2.75L;
  REAL_UPPER_BOUND = 1.75L;
  COMPLEX_LOWER_BOUND = -1.25L;
  COMPLEX_UPPER_BOUND = 1.25L;
}

void pan(int deltaX, int deltaY, int width, int height) {
  long double realScale = (REAL_UPPER_BOUND - REAL_LOWER_BOUND) / width;
  long double complexScale = (COMPLEX_UPPER_BOUND - COMPLEX_LOWER_BOUND) / height;
  
  long double realShift = -deltaX * realScale;
  long double complexShift = -deltaY * complexScale;
  
  REAL_LOWER_BOUND += realShift;
  REAL_UPPER_BOUND += realShift;
  COMPLEX_LOWER_BOUND += complexShift;
  COMPLEX_UPPER_BOUND += complexShift;
}