#include <iostream>

#ifdef __EMSCRIPTEN__
constexpr int LUT_SIZE = 2048;
#else
constexpr int LUT_SIZE = 4096;
#endif

struct RGBLUT {
    std::vector<uint8_t> r, g, b;
};

RGBLUT generate_color_lut();
const RGBLUT& get_color_lut();