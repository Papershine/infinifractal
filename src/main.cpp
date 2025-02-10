#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include <Draw.hpp>

int main(int argc, char **args)
{
  SDL_Surface *winSurface = NULL;
  SDL_Window *window = NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  window = SDL_CreateWindow("infinifractal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

  if (!window)
  {
    std::cout << "Error creating window: " << SDL_GetError() << std::endl;
    return 1;
  }

  winSurface = SDL_GetWindowSurface(window);

  if (!winSurface)
  {
    std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
    return 1;
  }

  auto start = std::chrono::high_resolution_clock::now();

  draw(winSurface);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Draw time: " << duration.count() << " ms\n";

  SDL_UpdateWindowSurface(window);

  SDL_Event e;
  bool quit = false;
  while (quit == false)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        quit = true;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}