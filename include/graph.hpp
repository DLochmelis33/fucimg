#pragma once
#include "common.hpp"

using Point = pair<int, int>;
using Edge = pair<Point, Point>; // <from, to>

vector<Edge> euclidean_mst(const vector<pair<int, int>>& points);