#include <iostream>

constexpr int LUT_SIZE = 4096;

struct RGBLUT {
    std::vector<uint8_t> r, g, b;
};

RGBLUT generateColorLUT();
const RGBLUT& getColorLUT();