#pragma once
#include "BmpImg.hpp"
#include "common.hpp"
#include <algorithm>
#include <deque>
#include <math.h>
#include <vector>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

void dfs_random_walk(BmpImg& img, vvbool& been, int x, int y,
                     Pixel* px = nullptr, double threshold = 3000);

void dfs_rw_stepK(BmpImg& img, vvbool& been, vector<pair<int, int>>& offsetSeq,
                  int x, int y, Pixel* px = nullptr, double threshold = 3000);

void bfs(BmpImg& img, double threshold = 3000);