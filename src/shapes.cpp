#include "shapes.hpp"

void drawCircle(vvpix& pixels, uint rad, int xc, int yc, Pixel color) {
    for (uint y = yc - rad; y < yc + rad; y++) {
        for (uint x = xc - rad; x < xc + rad; x++) {
            if (distSq({x, y}, {xc, yc}) <= rad * rad)
                if (inBounds(x, y, pixels.width, pixels.height))
                    pixels[y][x] = color;
        }
    }
}

void drawSquare(vvpix& pixels, int sqRad, int xc, int yc, Pixel color) {
    for (uint y = yc - sqRad; y < yc + sqRad; y++) {
        for (uint x = xc - sqRad; x < xc + sqRad; x++) {
            if (inBounds(x, y, pixels.width, pixels.height))
                pixels[y][x] = color;
        }
    }
}

void drawRectangle(vvpix& pixels, Point p1, Point p2, Pixel color) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);
    for (uint y = std::max(0, y1); y < std::min((int)pixels.height, y2); y++) {
        for (uint x = std::max(0, x1); y < std::min((int)pixels.width, x2); x++) {
            pixels[y][x] = color;
        }
    }
}

Point rotate(Point p, double angle) { // relative to (0, 0)
    auto [x, y] = p;
    int nx = std::round(x * std::cos(angle) - y * std::sin(angle));
    int ny = std::round(x * std::sin(angle) + y * std::cos(angle));
    return {nx, ny};
}

void drawRectangle(vvpix& pixels, int xc, int yc, uint w, uint h, double angle, Pixel color) {
    double w2 = w / 2.0;
    double h2 = h / 2.0;
    int maxOff = std::ceil(std::sqrt(w2 * w2 + h2 * h2));
    for (int yoff = -maxOff; yoff < maxOff; yoff++) {
        for (int xoff = -maxOff; xoff < maxOff; xoff++) {
            auto [ax, ay] = rotate({xoff, yoff}, -angle); // rotate the entire space backwards
            if (ax > -w2 && ax < w2 && ay > -h2 && ay < +h2) {
                int x = xc + xoff;
                int y = yc + yoff;
                if (inBounds(x, y, pixels.width, pixels.height))
                    pixels[y][x] = color;
            }
        }
    }
}

void drawSegment(vvpix& pixels, Point p1, Point p2, double width, Pixel color) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    drawCircle(pixels, width, x1, y1, color);
    drawCircle(pixels, width, x2, y2, color);
    double angle = std::atan(1.0 * (y1 - y2) / (x1 - x2));
    // ! DO NOT TOUCH THIS SHIT BELOW
    // if (y1 < y2)
    //     angle += M_PI;
    if (x1 > x2)
        angle += M_PI;
    angle += M_PI;
    // ! ---------
    uint step = 2;
    double length = sqrt(distSq({x1, y1}, {x2, y2}));
    for (uint cnt = 1; cnt < length / step; cnt++) {
        double deltaX = cnt * step * std::cos(angle);
        double deltaY = cnt * step * std::sin(angle);
        drawRectangle(pixels, x2 + deltaX, y2 + deltaY, 3 * step, width, angle, color);
    }
}