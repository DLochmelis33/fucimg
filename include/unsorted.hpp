#pragma once
#include "BmpImg.hpp"
#include "common.hpp"
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

void zakaz1(BmpImg& img);

void zakaz2(BmpImg& img);

void bounds_avgsigm(BmpImg& img, uint TH, int n = 5);

void mix_2kp1(BmpImg& img, unsigned int k); // i.e. mix_5 for k=2