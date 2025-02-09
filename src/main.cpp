#include <iostream>
#include <SDL2/SDL.h>
#include <Draw.h>

// You must include the command line parameters for your main function to be recognized by SDL
int main(int argc, char **args)
{

  // Pointers to our window and surface
  SDL_Surface *winSurface = NULL;
  SDL_Window *window = NULL;

  // Initialize SDL. SDL_Init will return -1 if it fails.
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    // End the program
    return 1;
  }

  // Create our window
  window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

  // Make sure creating the window succeeded
  if (!window)
  {
    std::cout << "Error creating window: " << SDL_GetError() << std::endl;
    // End the program
    return 1;
  }

  // Get the surface from the window
  winSurface = SDL_GetWindowSurface(window);

  // Make sure getting the surface succeeded
  if (!winSurface)
  {
    std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
    // End the program
    return 1;
  }

  //SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 197, 211));
  draw(winSurface, 1280, 720);
  SDL_UpdateWindowSurface(window);

  // Hack to get window to stay up
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