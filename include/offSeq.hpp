#pragma once
#include "common.hpp"

using OffSeq = vector<pair<int, int>>;

OffSeq osCircle(double rad, bool outline, double width = /*sqrt(2) / 2*/ 0.5);

OffSeq osSquare(int sqrad, bool outline); // sqrad = side of square / 2

OffSeq osAdjacent();

OffSeq osGaussian(int count, double sigma);