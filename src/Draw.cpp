#include <Draw.hpp>
#include <thread>
#include <iostream>

struct args {
  int y;
  int width;
  int height;
  std::mutex& buf_mutex;
  std::vector<Uint32>& buf;
};

void task(void* arg) {
  args *a = static_cast<args*>(arg);
  std::vector<Uint32> task_buffer;

  for (int x=0; x < a->width; x++) {
    task_buffer.push_back(calculateColor(coordsToComplex(x, a->y, a->width, a->height)));
  }

  {
    std::unique_lock lock(a->buf_mutex);
    std::copy(task_buffer.begin(), task_buffer.end(), a->buf.begin() + (a->y * a->width));
  }
  delete a;
}

void draw(SDL_Surface* surface)
{
  std::vector<Uint32> pixel_buffer(surface->w * surface->h);
  std::mutex buf_mutex;
  Threadpool pool;
  for (int y=0; y < surface->h; y++) {
    args* task_args = new args {y, surface->w, surface->h, buf_mutex, pixel_buffer};
    pool.schedule(task, task_args);
  }
  pool.join();
  Uint32* pixels = (Uint32*)surface->pixels;
  for (unsigned i=0; i<pixel_buffer.size(); i++) {
    pixels[i] = pixel_buffer[i];
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