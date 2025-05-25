#include <numbers>
#include <algorithm>
#include <Color.hpp>

RGBLUT generate_color_lut() {
    #ifndef __EMSCRIPTEN__
    auto start = std::chrono::high_resolution_clock::now();
    #endif

    RGBLUT lut;
    lut.r.resize(LUT_SIZE);
    lut.g.resize(LUT_SIZE);
    lut.b.resize(LUT_SIZE);

    for (int i = 0; i < LUT_SIZE; ++i) {
        double angle = (static_cast<double>(i) / static_cast<double>(LUT_SIZE)) * 2.0 * std::numbers::pi;

        double offset = 170.0;
        double amplitude = 255.0 - offset - 35.0;

        lut.r[i] = static_cast<uint8_t>(std::sin(angle) * amplitude + offset);
        lut.g[i] = static_cast<uint8_t>(std::sin(angle + 2.0 * std::numbers::pi / 3.0) * amplitude + offset);
        lut.b[i] = static_cast<uint8_t>(std::sin(angle + 4.0 * std::numbers::pi / 3.0) * amplitude + offset);
    }

    #ifndef __EMSCRIPTEN__
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "[LUT] Generated in " << duration.count() << "ms" << std::endl;
    #endif
    
    return lut;
}

const RGBLUT& get_color_lut() {
    static const RGBLUT lut = generate_color_lut();
    return lut;
}