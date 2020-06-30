#include "bdfs.hpp"

void dfs_random_walk_impl(BmpImg& img, const OffSeq& offSeq, vvbool& been, int x, int y, Pixel* px,
                          double threshold) {
    if (been[y][x])
        return;
    been[y][x] = true;
    if (!px)
        px = &img.pixels[y][x];

    random_shuffle(offSeq.begin(), offSeq.end());

    for (auto [xoffset, yoffset] : offSeq) {
        int newX = x + xoffset;
        int newY = y + yoffset;
        if (inBounds(newX, newY, img.width, img.height)) {
            if (colorDist(img.pixels[newY][newX], *px) <= threshold) {
                dfs_random_walk_impl(img, offSeq, been, newX, newY, px, threshold);
                img.pixels[newY][newX] = *px;
                // no need to copy since we have 'been'
            } else
                dfs_random_walk_impl(img, offSeq, been, newX, newY, nullptr, threshold);
        }
    }
}

void dfs_random_walk(BmpImg& img, OffSeq offSeq, double threshold, int start_x, int start_y) {
    vvbool been = vvbool(img.height, vector<bool>(img.width, false));
    for (uint y = 0; y < img.height; y++)
        for (uint x = 0; x < img.width; x++)
            if (!been[y][x])
                dfs_random_walk_impl(img, offSeq, been, start_x, start_y, nullptr, threshold);
}

void bfs_impl(BmpImg& img, const OffSeq& offSeq, double threshold, vvbool& been, int x, int y) {
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
            if (inBounds(newX, newY, img.width, img.height)) {
                if (colorDist(img.pixels[curY][curX], img.pixels[newY][newX]) < threshold) {
                    d.push_back({newX, newY});
                    img.pixels[newY][newX] = img.pixels[curY][curX];
                    // no need to copy since we have 'been'
                }
            }
        }
    }
}

void bfs(BmpImg& img, OffSeq offSeq, double threshold, int start_x, int start_y) {
    vvbool been(img.height, vector<bool>(img.width, false));
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            if (!been[y][x])
                bfs_impl(img, offSeq, threshold, been, x, y);
        }
    }
}