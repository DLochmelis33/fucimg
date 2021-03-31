#include "funcs.hpp"
#include <utility>

std::pair<double, double> px_to_real(int x, int y, int w, int h, double wscale, double hscale) {
    return {wscale * x / w, hscale * y / h};
}

void apply_color_func(vvpix& img, colorfunc fr, colorfunc fg, colorfunc fb, double wscale, double hscale) {
    int w = img.width, h = img.height;
    auto to_byte = [] (double val) { return (int) std::round(val) % 256; };
    for(int x = 0; x < w; ++x) {
        for(int y = 0; y < h; ++y) {
            auto [rx, ry] = px_to_real(x, y, w, h, wscale, hscale);
            img[y][x] = Pixel(to_byte(fb(rx, ry)), to_byte(fg(rx, ry)), to_byte(fr(rx, ry)));
        }
    }
}