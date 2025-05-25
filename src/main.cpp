#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include <Draw.hpp>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

int lastMouseX = 0, lastMouseY = 0;
bool isDragging = false;
bool quit = false;

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

#ifdef __EMSCRIPTEN__
int resScale = 4;
SDL_Surface *halfResSurface = nullptr;
SDL_Surface *quarterResSurface = nullptr;
std::atomic<bool> background_draw_allowed(false);
#endif

void mainloop(void *arg)
{
  SDL_Window *window = (SDL_Window *)arg;
  SDL_Surface *winSurface = SDL_GetWindowSurface(window);

  SDL_Event e;
  bool needRedraw = false;

  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      #ifdef __EMSCRIPTEN__
      emscripten_cancel_main_loop();
      #endif
      quit = true;
    }
    else if (e.type == SDL_MULTIGESTURE)
    {
      if (e.mgesture.numFingers == 2 && std::abs(e.mgesture.dDist) > 0.002f) {
        int touchX = static_cast<int>(e.mgesture.x * winSurface->w);
        int touchY = static_cast<int>(e.mgesture.y * winSurface->h);
        
        float zoomFactor = 1.0f;
        if (e.mgesture.dDist > 0) {
          zoomFactor = 0.95f;
        } else {
          zoomFactor = 1.05f;
        }
        
        zoom(touchX, touchY, zoomFactor, winSurface->w, winSurface->h);
        needRedraw = true;
      }
    }
    else if (e.type == SDL_MOUSEWHEEL)
    {
      float zoomFactor = 1.0f;
      if (e.wheel.y > 0)
      {
        zoomFactor = 0.8f;
      }
      else if (e.wheel.y < 0)
      {
        zoomFactor = 1.25f;
      }

      if (e.wheel.y != 0)
      {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        zoom(mouseX, mouseY, zoomFactor, winSurface->w, winSurface->h);
        needRedraw = true;
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        isDragging = true;
        SDL_GetMouseState(&lastMouseX, &lastMouseY);
      }
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        isDragging = false;
      }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
      if (isDragging)
      {
        int currentMouseX, currentMouseY;
        SDL_GetMouseState(&currentMouseX, &currentMouseY);

        int deltaX = currentMouseX - lastMouseX;
        int deltaY = currentMouseY - lastMouseY;

        if (deltaX != 0 || deltaY != 0)
        {
          pan(deltaX, deltaY, winSurface->w, winSurface->h);
          needRedraw = true;

          lastMouseX = currentMouseX;
          lastMouseY = currentMouseY;
        }
      }
    }
    else if (e.type == SDL_KEYDOWN)
    {
      if (e.key.keysym.sym == SDLK_r)
      {
        resetView();
        needRedraw = true;
      }
    }
    else if (e.type == SDL_WINDOWEVENT)
    {
      if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
          e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
      {
        winSurface = SDL_GetWindowSurface(window);

        long double aspectRatio = static_cast<long double>(winSurface->w) / static_cast<long double>(winSurface->h);
          
        long double realWidth = REAL_UPPER_BOUND - REAL_LOWER_BOUND;
        long double complexCenter = (COMPLEX_UPPER_BOUND + COMPLEX_LOWER_BOUND) / 2.0L;
          
        long double newComplexHeight = realWidth / aspectRatio;
          
        COMPLEX_LOWER_BOUND = complexCenter - (newComplexHeight / 2.0L);
        COMPLEX_UPPER_BOUND = complexCenter + (newComplexHeight / 2.0L);

        #ifdef __EMSCRIPTEN__
        SDL_FreeSurface(halfResSurface);
        halfResSurface = SDL_CreateRGBSurface(0, winSurface->w / 2, winSurface->h / 2, 32, 0, 0, 0, 0);
        SDL_FreeSurface(quarterResSurface);
        quarterResSurface = SDL_CreateRGBSurface(0, winSurface->w / 4, winSurface->h / 4, 32, 0, 0, 0, 0);
        #endif
        
        needRedraw = true;
      }
    }
  }

  if (needRedraw)
  {
    #ifdef __EMSCRIPTEN__
    resScale = 4;
    draw(quarterResSurface);
    SDL_Rect dstRect = {0, 0, winSurface->w, winSurface->h};
    SDL_BlitScaled(quarterResSurface, NULL, winSurface, &dstRect);
    #else
    auto start = std::chrono::high_resolution_clock::now();
    draw(winSurface);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Redraw time: " << duration.count() << " ms\n";
    #endif
    SDL_UpdateWindowSurface(window);
    needRedraw = false;
    #ifdef __EMSCRIPTEN__
    background_draw_allowed.store(false);
    #endif
  }
  #ifdef __EMSCRIPTEN__
  else if (resScale == 2)
  {
    resScale = 1;
    background_draw_allowed.store(true);
    std::thread t(draw_interruptible, winSurface, window, &background_draw_allowed);
    t.detach();
  }
  else if (resScale == 4)
  {
    resScale = 2;
    draw(halfResSurface);
    SDL_Rect dstRect = {0, 0, winSurface->w, winSurface->h};
    SDL_BlitScaled(halfResSurface, NULL, winSurface, &dstRect);
    SDL_UpdateWindowSurface(window);
  }
  #endif

  SDL_Delay(10);
}

int main()
{
  SDL_Surface *winSurface = NULL;
  SDL_Window *window = NULL;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
  {
    std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  window = SDL_CreateWindow("infinifractal",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINDOW_WIDTH, WINDOW_HEIGHT,
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

  SDL_RecordGesture(-1);
  
  #ifdef __EMSCRIPTEN__
  quarterResSurface = SDL_CreateRGBSurface(0, winSurface->w / 4, winSurface->h / 4, 32, 0, 0, 0, 0);
  halfResSurface = SDL_CreateRGBSurface(0, winSurface->w / 2, winSurface->h / 2, 32, 0, 0, 0, 0);
  draw(quarterResSurface);
  SDL_Rect dstRect = {0, 0, winSurface->w, winSurface->h};
  SDL_BlitScaled(quarterResSurface, NULL, winSurface, &dstRect);
  #else
  auto start = std::chrono::high_resolution_clock::now();
  draw(winSurface);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Draw time: " << duration.count() << " ms\n";
  #endif
  
  SDL_UpdateWindowSurface(window);

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(mainloop, window, 0, 1);
  #else
  while (quit == false)
  {
    mainloop(window);
  }
  #endif

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}