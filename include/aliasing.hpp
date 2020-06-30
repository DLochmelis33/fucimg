#pragma once
#include "common.hpp"
#include "offSeq.hpp"

void avg_4adjM(BmpImg& img);

void avg_os(BmpImg& img, OffSeq offSeq, int threshold = 0);

void ssaa4x(BmpImg& img);

void median_filter(
    BmpImg& img, std::function<bool(const Pixel&, const Pixel&)> comparator =
                     [](const Pixel& p1, const Pixel& p2) { return p1.sumSq() < p2.sumSq(); });