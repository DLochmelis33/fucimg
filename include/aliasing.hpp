#pragma once
#include "common.hpp"

void avg_4adjM(BmpImg& img);

vector<vector<Pixel>> avg_adjK(vector<vector<Pixel>> pixels, int height,
                               int width, int k);
void avg_adjK(BmpImg& img, int k);

void ssaa4x(BmpImg& img);

void avg_ifth(BmpImg& img, int TH, int k = 3);