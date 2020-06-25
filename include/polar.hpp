#pragma once
#include "BmpImg.hpp"
#include "common.hpp"
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

tuple<double, double, double> toPolar(double x, double y, double z);

tuple<double, double, double> fromPolar(double rad, double zen, double azi);

void polar_addCoord(BmpImg& img, double dRad, double dZen, double dAzi);

void polar_mulCoord(BmpImg& img, double dRad, double dZen, double dAzi);
