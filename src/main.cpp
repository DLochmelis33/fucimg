#include "BmpImg.hpp"
#include "common.hpp"
#include "th.hpp"

#include "aliasing.hpp"
#include "bdfs.hpp"
#include "polar.hpp"
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

    // -------

    dfs_random_walk(img, osGaussian(K1, VAR1), TH1);
    median_filter(img, VAR2);

    // -------

    // merge(img1, img2, img3);
    // avg_ifth(img3, TH2, K2);

    // * interesting stuff over

    int ms_elapsed = (clock() - start_time) * 1000.0 / CLOCKS_PER_SEC;
    cout << "ended in " << (ms_elapsed / 1000.0) << " seconds" << endl;

    return 0;
}