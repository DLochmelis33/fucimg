#pragma once
#include "common.hpp"
#include "offSeq.hpp"

void dfs_random_walk(vvpix& pixels, OffSeq offSeq, double threshold);

void bfs(vvpix& pixels, OffSeq offSeq, double threshold);