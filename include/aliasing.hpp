#pragma once
#include "common.hpp"
#include "offSeq.hpp"

void avg_4adjM(vvpix& img);

void avg_os(vvpix& img, OffSeq offSeq, int threshold = 0);

void ssaa4x(vvpix& img);

void median_filter(
    vvpix& img, double rad, std::function<bool(const Pixel&, const Pixel&)> comparator =
                     [](const Pixel& p1, const Pixel& p2) { return p1.sumSq() < p2.sumSq(); });