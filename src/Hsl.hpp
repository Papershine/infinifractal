#pragma once

#include <algorithm>
#include <SDL2/SDL.h>

class Hsl {
  public:
    double h;
    double s;
    double l;

    Hsl(double hue, double sat, double light) : h(hue), s(sat), l(light) {};
    Uint32 toRgb();

  private:
    uint8_t toRgbHelper(int n, double a);
};