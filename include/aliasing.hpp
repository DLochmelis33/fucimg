#pragma once
#include "BmpImg.hpp"
#include "common.hpp"
#include <math.h>
#include <vector>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

using vvbool = vector<vector<bool>>;

void avg_4adjM(BmpImg& img);

vector<vector<Pixel>> avg_adjK(vector<vector<Pixel>> pixels, int height,
                               int width, int k);
void avg_adjK(BmpImg& img, int k);

void ssaa4x(BmpImg& img);

void avg_ifth(BmpImg& img, int TH, int k = 3);