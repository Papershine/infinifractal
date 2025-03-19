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

  window = SDL_CreateWindow("infinifractal", 
                            SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED, 
                            1280, 720, 
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
  bool needRedraw = false;
  
  int lastMouseX = 0, lastMouseY = 0;
  
  bool isDragging = false;
  
  while (quit == false)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        quit = true;
      else if (e.type == SDL_MOUSEWHEEL)
      {
        float zoomFactor = 1.0f;
        if (e.wheel.y > 0) {
          zoomFactor = 0.8f;
        } else if (e.wheel.y < 0) {
          zoomFactor = 1.25f;
        }
        
        if (e.wheel.y != 0) {
          int mouseX, mouseY;
          SDL_GetMouseState(&mouseX, &mouseY);
          zoom(mouseX, mouseY, zoomFactor, winSurface->w, winSurface->h);
          needRedraw = true;
        }
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
          isDragging = true;
          SDL_GetMouseState(&lastMouseX, &lastMouseY);
        }
      }
      else if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == SDL_BUTTON_LEFT) {
          isDragging = false;
        }
      }
      else if (e.type == SDL_MOUSEMOTION) {
        if (isDragging) {
          int currentMouseX, currentMouseY;
          SDL_GetMouseState(&currentMouseX, &currentMouseY);
          
          int deltaX = currentMouseX - lastMouseX;
          int deltaY = currentMouseY - lastMouseY;
          
          if (deltaX != 0 || deltaY != 0) {
            pan(deltaX, deltaY, winSurface->w, winSurface->h);
            needRedraw = true;
            
            lastMouseX = currentMouseX;
            lastMouseY = currentMouseY;
          }
        }
      }
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_r:
            resetView();
            needRedraw = true;
            break;
          default:
            break;
        }
      }
      else if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED || 
            e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
          // Get the new window surface
          winSurface = SDL_GetWindowSurface(window);
          if (winSurface) {
            std::cout << "Window resized to " << winSurface->w << "x" << winSurface->h << "\n";
            needRedraw = true;
          }
        }
      }
    }
    
    if (needRedraw) {
      auto start = std::chrono::high_resolution_clock::now();
      draw(winSurface);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> duration = end - start;
      std::cout << "Redraw time: " << duration.count() << " ms\n";
      SDL_UpdateWindowSurface(window);
      needRedraw = false;
    }
    
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}