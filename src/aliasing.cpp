#include "aliasing.hpp"
#include "offSeq.hpp"

void avg_os(vvpix& pixels, OffSeq offSeq, int threshold) {
    vvpix newPixels = pixels;
    uint height = pixels.size();
    assert(height > 0);
    uint width = pixels[0].size();
    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, width, height))
                    adj.push_back(pixels[curY][curX]);
            }
            Pixel avg = averagePixel(adj);
            if (colorDist(pixels[y][x], avg) > threshold)
                newPixels[y][x] = avg;
        }
    }
    pixels = newPixels;
}

void ssaa4x(BmpImg& img) {
    vector<vector<Pixel>> buffer(2 * img.height, vector<Pixel>(2 * img.width, {0, 0, 0}));

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            buffer[2 * y][2 * x] = img[y][x];
            buffer[2 * y][2 * x + 1] = img[y][x];
            buffer[2 * y + 1][2 * x] = img[y][x];
            buffer[2 * y + 1][2 * y + 1] = img[y][x];
        }
    }

    avg_os(buffer, osSquare(2, false), 0);

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            img[y][x] = averagePixel({buffer[2 * y][2 * x], buffer[2 * y + 1][2 * x],
                                      buffer[2 * y][2 * x + 1], buffer[2 * y + 1][2 * x + 1]});
        }
    }
}

void median_filter(BmpImg& img, double rad,
                   std::function<bool(const Pixel&, const Pixel&)> comparator) {
    vvpix newPixels = img.pixels;
    OffSeq offSeq = osCircle(rad, false);
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, img.width, img.height))
                    adj.push_back(img.pixels[curY][curX]);
            }
            assert(adj.size() > 0);
            std::nth_element(adj.begin(), adj.begin() + adj.size() / 2, adj.end(), comparator);
            newPixels[y][x] = adj[adj.size() / 2];
        }
    }
    img.pixels = newPixels;
}