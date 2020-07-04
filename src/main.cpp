// #define UID_NAMES

#include "BmpImg.hpp"
#include "common.hpp"
#include "th.hpp"

#include "aliasing.hpp"
#include "bdfs.hpp"
#include "graph.hpp"
#include "polar.hpp"
#include "shapes.hpp"
#include "unsorted.hpp"

#include <bits/stdc++.h>
#include <sys/resource.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char** argv) {
    rlimit my_stack_limit{(1LL << 30), (1LL << 32)};
    setrlimit(RLIMIT_STACK, &my_stack_limit);

    if (argc < 2) {
        cerr << "not enough arguments" << endl;
        exit(1);
    }

    cout << "started..." << endl;
    clock_t start_time = clock();

    BmpImg img(argv[1]);

    // * interesting stuff here

    // mix_2kp1(img, 3);
    // dfs_random_walk(img, vec, 0, 0);
    // bfs(img);
    // bounds_avgsigm(img, TH1, K1);

    // ssaa4x(img);

    // polar_addCoord(img, VAR1, VAR2, VAR3);
    // polar_mulCoord(img, VAR4, VAR5, VAR6);

    // dfs_random_walk(img, osGaussian(K1, VAR1), TH1);
    // median_filter(img, VAR2);

    // srand(time(0));
    // color_k_means(img, K1, VAR1, TH1);
    // median_filter(img, VAR2);
    // median_filter(img, VAR3);

    // -------

    zakaz2(img);
    median_filter(img, VAR3);
    vector<Point> points;
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            if (img[y][x].B == 0 && rand() % 10 == 0)
                points.push_back({y, x});
            img[y][x] = 255;
        }
    }
    vector<Edge> segments = euclidean_mst(points);
    for (Edge seg : segments) {
        auto [p1, p2] = seg;
        drawSegment(img, p1, p2, VAR4, {0, 0, 0});
    }

    // ssaa4x(img);

    // -------

    // merge(img1, img2, img3);
    // avg_ifth(img3, TH2, K2);

    // * interesting stuff over

    int ms_elapsed = (clock() - start_time) * 1000.0 / CLOCKS_PER_SEC;
    cout << "ended in " << (ms_elapsed / 1000.0) << " seconds" << endl;

    return 0;
}