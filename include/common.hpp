#pragma once
#include "BmpImg.hpp"
#include <bits/stdc++.h>

using namespace std;

using Pixel = BmpImg::Pixel;
using uint = unsigned int;
using vvbool = vector<vector<bool>>;
using vvpix = vector<vector<Pixel>>;

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

inline Pixel averagePixel(const vector<Pixel>& pixels) {
    int n = pixels.size();
    double sumB = 0;
    double sumG = 0;
    double sumR = 0;
    for (const Pixel& p : pixels) {
        sumB += p.B;
        sumG += p.G;
        sumR += p.R;
    }
    return Pixel(round(sumB / n), round(sumG / n), round(sumR / n));
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

inline void merge(
    vvpix& vec1, vvpix& vec2, vvpix& vec3,
    std::function<Pixel(const Pixel&, const Pixel&)> mergeRule =
        [](const Pixel& p1, const Pixel& p2) {
            return averagePixel({p1, p2});
        }) {

    uint vec1height = vec1.size();
    assert(vec1height > 0);
    uint vec1width = vec1[0].size();

    uint vec2height = vec2.size();
    assert(vec2height > 0);
    uint vec2width = vec2[0].size();

    uint vec3height = vec3.size();
    assert(vec3height > 0);
    uint vec3width = vec3[0].size();

    assert(vec1height == vec2height && vec2height == vec3height);
    assert(vec1width == vec2width && vec2width == vec3width);

    for (uint y = 0; y < vec1height; y++) {
        for (uint x = 0; x < vec1width; x++) {
            vec3[y][x] = mergeRule(vec1[y][x], vec2[y][x]);
        }
    }
}