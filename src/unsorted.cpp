#include "unsorted.hpp"

void zakaz1(BmpImg& img) {
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            Pixel& p = img[y][x];
            if ((p.B + p.G + p.R) / 3 < 127)
                p = 0;
            else
                p = 255;
        }
    }
}

void zakaz2(BmpImg& img) {
    unsigned long long sumB = 0;
    unsigned long long sumG = 0;
    unsigned long long sumR = 0;
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            sumB += img[y][x].B;
            sumG += img[y][x].G;
            sumR += img[y][x].R;
        }
    }
    unsigned char avgB = sumB / (img.height * img.width);
    unsigned char avgG = sumG / (img.height * img.width);
    unsigned char avgR = sumR / (img.height * img.width);
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            Pixel& p = img[y][x];
            if (colorDist(p, {0, 0, 0}) <
                colorDist({avgB, avgG, avgR}, {0, 0, 0}))
                p = 0;
            else
                p = 255;
        }
    }
}

void bounds_avgsigm(BmpImg& img, uint TH, int n) {
    // Sigmoid sigm({THRESHOLD});
    vector<pair<int, int>> offset = genOffsetSeqSquareN(n);

    vector<vector<Pixel>> new_pixels = img.pixels;
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            const Pixel& px = img[y][x];
            unsigned int sumDiffSq = 0;
            double cnt = 0;
            for (auto [xoff, yoff] : offset) {
                int cur_x = x + xoff;
                int cur_y = y + yoff;
                if (inBounds(cur_x, cur_y, img.width, img.height)) {
                    const Pixel& curpx = img[cur_y][cur_x];
                    sumDiffSq += (curpx.B - px.B) * (curpx.B - px.B) +
                                 (curpx.G - px.G) * (curpx.G - px.G) +
                                 (curpx.R - px.R) * (curpx.R - px.R);
                    cnt++;
                }
            }
            // unsigned char new_value = sigm(sumDiffSq / cnt) * 256;
            unsigned char new_value = 0;
            if (sumDiffSq < TH)
                new_value = 255;
            // if (y == x && x % 30 == 0)
            //     cout << sumDiffSq << ' ' << th << ' ' << (unsigned
            //     int)new_value << endl;

            new_pixels[y][x] = new_value;
        }
    }
    img.pixels = new_pixels;
}

void mix_2kp1(BmpImg& img, unsigned int k) { // i.e. mix_5 for k=2
    // static double constexpr THRESHOLD = 3000;
    unsigned int n = 2 * k + 1;
    vector<int> permut;
    for (uint i = 0; i < n * n; i++)
        permut.push_back(i);

    for (uint yc = k; yc < img.height - k; yc++) {
        // cout << yc << endl;
        for (uint xc = k; xc < img.width - k; xc++) {

            random_shuffle(permut.begin(), permut.end());
            for (uint oldind = 0; oldind < n * n; oldind++) {
                int newind = permut[oldind];

                int oldx_ = oldind % n;
                int oldy_ = oldind / n;

                int newx_ = newind % n;
                int newy_ = newind / n;

                int oldX = xc - k + oldx_;
                int oldY = yc - k + oldy_;

                int newX = xc - k + newx_;
                int newY = yc - k + newy_;

                swap(img.pixels[oldY][oldX], img.pixels[newY][newX]);
            }
        }
    }
}