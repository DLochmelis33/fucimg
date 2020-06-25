#include "aliasing.hpp"

void avg_4adjM(BmpImg& img) {
    vector<vector<Pixel>> newPixels = img.pixels;
    vector<pair<int, int>> offSeq = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            int sumB = 0;
            int sumG = 0;
            int sumR = 0;
            double cnt = 0;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, img.width, img.height)) {
                    auto [curB, curG, curR] = img.pixels[curY][curX];
                    auto [B, G, R] = img.pixels[y][x];
                    sumB += curB + B;
                    sumG += curG + G;
                    sumR += curR + R;
                    cnt += 2;
                }
            }
            assert(cnt > 0);
            newPixels[y][x] = Pixel(sumB / cnt, sumG / cnt, sumR / cnt);
        }
    }
    img.pixels = newPixels;
}

vector<vector<Pixel>> avg_adjK(vector<vector<Pixel>> pixels, int height,
                               int width, int k) {
    vector<vector<Pixel>> newPixels = pixels;
    vector<pair<int, int>> offSeq = genOffsetSeqSquareN(k);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto [B, G, R] = pixels[y][x];
            int sumB = B;
            int sumG = G;
            int sumR = R;
            double cnt = 1;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, width, height)) {
                    auto [curB, curG, curR] = pixels[curY][curX];
                    sumB += curB;
                    sumG += curG;
                    sumR += curR;
                    cnt++;
                }
            }
            newPixels[y][x] = Pixel(sumB / cnt, sumG / cnt, sumR / cnt);
        }
    }
    return newPixels;
}

void avg_adjK(BmpImg& img, int k) {
    img.pixels = avg_adjK(img.pixels, img.height, img.width, k);
}

void ssaa4x(BmpImg& img) {
    vector<vector<Pixel>> buffer(2 * img.height,
                                 vector<Pixel>(2 * img.width, {0, 0, 0}));

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            buffer[2 * y][2 * x] = img[y][x];
            buffer[2 * y][2 * x + 1] = img[y][x];
            buffer[2 * y + 1][2 * x] = img[y][x];
            buffer[2 * y + 1][2 * y + 1] = img[y][x];
        }
    }

    avg_adjK(buffer, 2 * img.height, 2 * img.width, 31);

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            img[y][x] = averagePixel(
                {buffer[2 * y][2 * x], buffer[2 * y + 1][2 * x],
                 buffer[2 * y][2 * x + 1], buffer[2 * y + 1][2 * x + 1]});
        }
    }
}

void avg_ifth(BmpImg& img, int TH, int k) {
    vector<vector<Pixel>> newPixels = img.pixels;
    vector<pair<int, int>> offSeq = genOffsetSeqSquareN(k);
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, img.width, img.height))
                    adj.push_back(img[curY][curX]);
            }
            Pixel avg = averagePixel(adj);
            if (colorDist(img[y][x], avg) > TH)
                newPixels[y][x] = avg;
        }
    }
    img.pixels = newPixels;
}