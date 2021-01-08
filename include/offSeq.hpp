#pragma once
#include "common.hpp"

using OffSeq = vector<pair<int, int>>;

OffSeq osCircle(double rad, bool outline, double width = /*sqrt(2) / 2*/ 0.5);

OffSeq osSquare(int sqrad, bool outline); // sqrad = side of square / 2

OffSeq osAdjacent();

OffSeq osGaussian(int count, double sigma);

vector<Pixel> get_os_pixels(const vvpix& img, OffSeq offSeq, int x, int y);

Pixel avg_os(const vvpix& img, OffSeq offSeq, int x, int y);

Pixel median_os(const vvpix& img, OffSeq offSeq, int x, int y);