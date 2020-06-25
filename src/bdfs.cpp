#include "bdfs.hpp"

void dfs_random_walk(BmpImg& img, vvbool& been, int x, int y,
                     Pixel* px , double threshold) {
    auto offsetSequence = genOffsetSeqSquareN(3);
    if (been[y][x])
        return;
    been[y][x] = true;
    if (!px)
        px = &img.pixels[y][x];

    random_shuffle(offsetSequence.begin(), offsetSequence.end());

    for (auto [xoffset, yoffset] : offsetSequence) {
        int newX = x + xoffset;
        int newY = y + yoffset;
        if (inBounds(newX, newY, img.width, img.height)) {
            if (colorDist(img.pixels[newY][newX], *px) <= threshold) {
                dfs_random_walk(img, been, newX, newY, px);
                img.pixels[newY][newX] = *px;
            } else
                dfs_random_walk(img, been, newX, newY, nullptr);
        }
    }
}

void dfs_rw_stepK(BmpImg& img, vvbool& been, vector<pair<int, int>>& offsetSeq,
                  int x, int y, Pixel* px, double threshold) {
    if (been[y][x])
        return;
    been[y][x] = true;
    if (!px)
        px = &img.pixels[y][x];

    random_shuffle(offsetSeq.begin(), offsetSeq.end());

    for (auto [xoffset, yoffset] : offsetSeq) {
        int newX = x + xoffset;
        int newY = y + yoffset;
        if (inBounds(newX, newY, img.width, img.height)) {
            if (colorDist(img.pixels[newY][newX], *px) <= threshold) {
                dfs_rw_stepK(img, been, offsetSeq, newX, newY, px);
                img.pixels[newY][newX] = *px;
            } else
                dfs_rw_stepK(img, been, offsetSeq, newX, newY, nullptr);
        }
    }
}

void bfs(BmpImg& img, double threshold) {
    vvbool been(img.height, vector<bool>(img.width, false));
    auto offsetSequence = genOffsetSeqSquareN(3);

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            if (been[y][x])
                continue;
            // actual bfs
            deque<pair<int, int>> d;
            d.push_back({x, y});
            while (!d.empty()) {
                auto [curX, curY] = d.front();
                d.pop_front();
                if (been[curY][curX])
                    continue;
                been[curY][curX] = true;

                for (auto [xoffset, yoffset] : offsetSequence) {
                    int newX = curX + xoffset;
                    int newY = curY + yoffset;
                    if (inBounds(newX, newY, img.width, img.height)) {
                        if (colorDist(img.pixels[curY][curX],
                                      img.pixels[newY][newX]) < threshold) {
                            d.push_back({newX, newY});
                            img.pixels[newY][newX] = img.pixels[curY][curX];
                        }
                    }
                }
            }
        }
    }
}