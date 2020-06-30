#pragma once
#include "common.hpp"
#include "offSeq.hpp"

void dfs_random_walk(BmpImg &img, OffSeq offSeq, double threshold, int start_x = 0, int start_y = 0);

void bfs(BmpImg &img, OffSeq offSeq, double threshold, int start_x = 0, int start_y = 0);