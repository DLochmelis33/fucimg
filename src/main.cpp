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

    // vvbool vec(img.height, vector<bool>(img.width, false));
    // auto offsetSeq = genOffsetSeqStepK(20);
    // dfs_rw_stepK(img, vec, offsetSeq, 0, 0);
    // mix_2kp1(img, 3);
    // dfs_random_walk(img, vec, 0, 0);
    // bfs(img);
    // bounds_avgsigm(img, TH1, K1);

    // avg_4adjM(img);
    // avg_adjK(img, 3);
    // ssaa4x(img);

    // -------

    polar_addCoord(img, VAR1, VAR2, VAR3);
    polar_mulCoord(img, VAR4, VAR5, VAR6);

    // -------

    // zakaz2(img);

    // BmpImg img3(argv[1]);
    // BmpImg img1(argv[1]);
    // BmpImg img2(argv[1]);

    // zakaz1(img1);
    // zakaz2(img2);

    // merge(img1, img2, img3);
    // avg_ifth(img3, TH2, K2);

    // * interesting stuff over

    int ms_elapsed = (clock() - start_time) * 1000.0 / CLOCKS_PER_SEC;
    cout << "ended in " << (ms_elapsed / 1000.0) << " seconds" << endl;

    return 0;
}