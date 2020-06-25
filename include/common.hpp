#pragma once
#include "BmpImg.hpp"
#include <math.h>
#include <vector>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

using vvbool = vector<vector<bool>>;

inline double colorDist(const Pixel& p1, const Pixel& p2) {
    return (p1.B - p2.B) * (p1.B - p2.B) + (p1.G - p2.G) * (p1.G - p2.G) +
           (p1.R - p2.R) * (p1.R - p2.R);
}

inline double dist(pair<int, int> p1, pair<int, int> p2) {
    return (p1.first - p2.first) * (p1.first - p2.first) +
           (p1.second - p2.second) * (p1.second - p2.second);
}

inline bool inBounds(int x, int y, int w, int h) {
    return (x >= 0) && (x < w) && (y >= 0) && (y < h);
}

inline vector<pair<int, int>> genOffsetSeqStepK(int k) { // k - circle radius
    // outer circle shape
    vector<pair<int, int>> res;
    double eps = 0.5;
    for (int xoffset = -k; xoffset <= k; xoffset++) {
        for (int yoffset = -k; yoffset <= k; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            if (dist({xoffset, yoffset}, {0, 0}) - k * k < eps) {
                res.push_back({xoffset, yoffset});
            }
        }
    }
    return res;
}

inline vector<pair<int, int>> genOffsetSeqSquareN(int n) { // n - square side
    // filled square shape
    assert(n % 2 == 1);
    int k = n / 2;
    vector<pair<int, int>> res;
    for (int xoffset = -k; xoffset <= k; xoffset++) {
        for (int yoffset = -k; yoffset <= k; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            res.push_back({xoffset, yoffset});
        }
    }
    return res;
}

inline Pixel averagePixel(vector<Pixel> pixels) {
    int n = pixels.size();
    int sumB = 0;
    int sumG = 0;
    int sumR = 0;
    for (const Pixel& p : pixels) {
        sumB += p.B;
        sumG += p.G;
        sumR += p.R;
    }
    return Pixel(sumB / n, sumG / n, sumR / n);
}

struct Sigmoid {
    unsigned int limit_;
    double base_ = 2.7;
    Sigmoid(unsigned int limit) : limit_(limit){};
    Sigmoid(unsigned int limit, double base) : limit_(limit), base_(base){};
    double operator()(double x) const {
        return 1.0 / (1 + pow(base_, -(x - limit_ / 2)));
    }
};

inline void merge(BmpImg& img1, BmpImg& img2, BmpImg& img3) {
    assert(img1.height == img2.height && img2.height == img3.height);
    assert(img1.width == img2.width && img2.width == img3.width);

    for (uint y = 0; y < img1.height; y++) {
        for (uint x = 0; x < img1.width; x++) {
            // if (rand() % 2)
            //     img3[y][x] = img1[y][x];
            // else
            //     img3[y][x] = img2[y][x];
            img3[y][x] = averagePixel({img1[y][x], img2[y][x]});
        }
    }
}