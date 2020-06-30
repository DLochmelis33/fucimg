#include "bdfs.hpp"
#include "offSeq.hpp"
using std::deque;

void dfs_random_walk_impl(vvpix& pixels, OffSeq offSeq, vvbool& been, int x, int y, Pixel* px,
                          double threshold) {
    if (been[y][x])
        return;
    been[y][x] = true;
    if (!px)
        px = &pixels[y][x];

    random_shuffle(offSeq.begin(), offSeq.end());

    for (auto [xoffset, yoffset] : offSeq) {
        int newX = x + xoffset;
        int newY = y + yoffset;
        if (inBounds(newX, newY, pixels.width, pixels.height)) {
            if (colorDist(pixels[newY][newX], *px) <= threshold) {
                dfs_random_walk_impl(pixels, offSeq, been, newX, newY, px, threshold);
                pixels[newY][newX] = *px;
                // no need to copy since we have 'been'
            } else
                dfs_random_walk_impl(pixels, offSeq, been, newX, newY, nullptr, threshold);
        }
    }
}

void dfs_random_walk(vvpix& pixels, OffSeq offSeq, double threshold) {
    vvbool been = vvbool(pixels.height, vector<bool>(pixels.width, false));
    for (uint y = 0; y < pixels.height; y++)
        for (uint x = 0; x < pixels.width; x++)
            if (!been[y][x])
                dfs_random_walk_impl(pixels, offSeq, been, x, y, nullptr, threshold);
}

void bfs_impl(vvpix& pixels, const OffSeq& offSeq, double threshold, vvbool& been, int x, int y) {
    deque<pair<int, int>> d;
    d.push_back({x, y});
    while (!d.empty()) {
        auto [curX, curY] = d.front();
        d.pop_front();
        if (been[curY][curX])
            continue;
        been[curY][curX] = true;

        for (auto [xoffset, yoffset] : offSeq) {
            int newX = curX + xoffset;
            int newY = curY + yoffset;
            if (inBounds(newX, newY, pixels.width, pixels.height)) {
                if (colorDist(pixels[curY][curX], pixels[newY][newX]) < threshold) {
                    d.push_back({newX, newY});
                    pixels[newY][newX] = pixels[curY][curX];
                    // no need to copy since we have 'been'
                }
            }
        }
    }
}

void bfs(vvpix& pixels, OffSeq offSeq, double threshold) {
    vvbool been(pixels.height, vector<bool>(pixels.width, false));
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            if (!been[y][x])
                bfs_impl(pixels, offSeq, threshold, been, x, y);
        }
    }
}