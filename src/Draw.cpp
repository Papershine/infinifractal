#include <Draw.hpp>
#include <thread>
#include <iostream>

long double REAL_LOWER_BOUND = -2.75L;
long double REAL_UPPER_BOUND = 1.75L;
long double COMPLEX_LOWER_BOUND = -1.25L;
long double COMPLEX_UPPER_BOUND = 1.25L;

void task(int y, int width, int height, Uint32* buf, const SDL_PixelFormat *format) {
  std::vector<Uint32> task_buffer(width);

  for (int x=0; x < width; x++) {
    task_buffer[x] = calculateColor(coordsToComplex(x, y, width, height), format);
  }

  {
    std::copy(task_buffer.begin(), task_buffer.end(), buf + (y * width));
  }
}

void draw(SDL_Surface* surface)
{
  Threadpool pool;

  for (int y=0; y < surface->h; y++) {
    pool.schedule(task, y, surface->w, surface->h, (Uint32*)surface->pixels, surface->format);
  }

  pool.join();
}

void draw_line_interruptible(int y, int width, int height, Uint32* buf, const SDL_PixelFormat *format, std::atomic<bool>* background_draw_allowed) {
  std::vector<Uint32> task_buffer(width);

  for (int x=0; x < width; x++) {
    if (!(*background_draw_allowed).load()) {
      return;
    }
    task_buffer[x] = calculateColor(coordsToComplex(x, y, width, height), format);
  }

  std::copy(task_buffer.begin(), task_buffer.end(), buf + (y * width));
}

void draw_interruptible(SDL_Surface* surface, SDL_Window* window, std::atomic<bool>* background_draw_allowed)
{
  Uint32* pixel_buffer = new Uint32[surface->w * surface->h];
  Threadpool pool;

  for (int y=0; y < surface->h; y++) {
    pool.schedule(draw_line_interruptible, y, surface->w, surface->h, pixel_buffer, surface->format, background_draw_allowed);
  }

  pool.join();

  if ((*background_draw_allowed).load()) {
    std::copy(pixel_buffer, pixel_buffer + surface->w * surface->h, (Uint32*)surface->pixels);
    SDL_UpdateWindowSurface(window);
  }
  delete[] pixel_buffer;
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