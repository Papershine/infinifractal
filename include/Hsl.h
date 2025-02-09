#pragma once

#include <SDL2/SDL.h>

class Hsl {
  public:
    uint16_t h;
    double s;
    double l;

    Hsl(uint16_t hue, double sat, double light) : h(hue), s(sat), l(light) {};
    Uint32 toRgb();

  private:
    uint8_t toRgbHelper(int n, double a);
};