#include "aliasing.hpp"
#include "offSeq.hpp"

void avg_os(vvpix& pixels, OffSeq offSeq, int threshold) {
    vvpix newPixels = pixels;
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, pixels.width, pixels.height))
                    adj.push_back(pixels[curY][curX]);
            }
            Pixel avg = averagePixel(adj);
            if (colorDist(pixels[y][x], avg) > threshold)
                newPixels[y][x] = avg;
        }
    }
    pixels = newPixels;
}

void ssaa4x(vvpix& pixels) {
    vvpix buffer(vector<vector<Pixel>>(2 * pixels.height, vector<Pixel>(2 * pixels.width, {0, 0, 0})));

    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            buffer[2 * y][2 * x] = pixels[y][x];
            buffer[2 * y][2 * x + 1] = pixels[y][x];
            buffer[2 * y + 1][2 * x] = pixels[y][x];
            buffer[2 * y + 1][2 * y + 1] = pixels[y][x];
        }
    }

    avg_os(buffer, osSquare(2, false), 0);

    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            pixels[y][x] = averagePixel({buffer[2 * y][2 * x], buffer[2 * y + 1][2 * x],
                                      buffer[2 * y][2 * x + 1], buffer[2 * y + 1][2 * x + 1]});
        }
    }
}

void median_filter(vvpix& pixels, double rad,
                   std::function<bool(const Pixel&, const Pixel&)> comparator) {
    vvpix newPixels(pixels);
    OffSeq offSeq = osCircle(rad, false);
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, pixels.width, pixels.height))
                    adj.push_back(pixels[curY][curX]);
            }
            assert(adj.size() > 0);
            std::nth_element(adj.begin(), adj.begin() + adj.size() / 2, adj.end(), comparator);
            newPixels[y][x] = adj[adj.size() / 2];
        }
    }
    pixels = newPixels;
}