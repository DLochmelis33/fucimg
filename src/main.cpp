#include "bmp.hpp"
#include "th.hpp"

#include <bits/stdc++.h>

#include <sys/resource.h>
#include <sys/time.h>

using namespace std;
using Pixel = BmpImg::Pixel;
using uint = unsigned int;

using vvbool = vector<vector<bool>>;

// double constexpr THRESHOLD = 3000;

double colorDist(const Pixel& p1, const Pixel& p2) {
    return (p1.B - p2.B) * (p1.B - p2.B) + (p1.G - p2.G) * (p1.G - p2.G) +
           (p1.R - p2.R) * (p1.R - p2.R);
}

double dist(pair<int, int> p1, pair<int, int> p2) {
    return (p1.first - p2.first) * (p1.first - p2.first) +
           (p1.second - p2.second) * (p1.second - p2.second);
}

bool inBounds(int x, int y, int w, int h) { return (x >= 0) && (x < w) && (y >= 0) && (y < h); }

vector<pair<int, int>> offsetSequence = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                         {0, 1},   {1, -1}, {1, 0},  {1, 1}};

void dfs_random_walk(BmpImg& img, vvbool& been, int x, int y, Pixel* px = nullptr) {
    static double constexpr THRESHOLD = 3000;
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
            if (colorDist(img.pixels[newY][newX], *px) <= THRESHOLD) {
                dfs_random_walk(img, been, newX, newY, px);
                img.pixels[newY][newX] = *px;
            } else
                dfs_random_walk(img, been, newX, newY, nullptr);
        }
    }
}

vector<pair<int, int>> genOffsetSeqStepK(int k) { // k - circle radius
    // outer circle shape
    vector<pair<int, int>> res;
    double eps = 0.5;
    for (int xoffset = -k; xoffset <= k; xoffset++) {
        for (int yoffset = -k; yoffset <= k; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            if (dist({xoffset, yoffset}, {0, 0}) - k * k < eps) {
                res.push_back({xoffset, yoffset});
            }
        }
    }
    return res;
}

vector<pair<int, int>> genOffsetSeqSquareN(int n) { // n - square side
    // filled square shape
    assert(n % 2 == 1);
    int k = n / 2;
    vector<pair<int, int>> res;
    for (int xoffset = -k; xoffset <= k; xoffset++) {
        for (int yoffset = -k; yoffset <= k; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            res.push_back({xoffset, yoffset});
        }
    }
    return res;
}

tuple<double, double, double> toPolar(double x, double y, double z) {
    double rad = sqrt(x * x + y * y + z * z);
    double zen = (z == 0 ? M_PI / 2 : atan(sqrt(x * x + y * y) / z));
    double azi = (x == 0 ? M_PI / 2 : atan(y / x));
    return {rad, zen, azi};
}

tuple<double, double, double> fromPolar(double rad, double zen, double azi) {
    double x = rad * sin(zen) * cos(azi);
    double y = rad * sin(zen) * sin(azi);
    double z = rad * cos(zen);
    return {x, y, z};
}

void dfs_rw_stepK(BmpImg& img, vvbool& been, vector<pair<int, int>>& offsetSeq, int x, int y,
                  Pixel* px = nullptr) {
    static double constexpr THRESHOLD = 3000;
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
            if (colorDist(img.pixels[newY][newX], *px) <= THRESHOLD) {
                dfs_rw_stepK(img, been, offsetSeq, newX, newY, px);
                img.pixels[newY][newX] = *px;
            } else
                dfs_rw_stepK(img, been, offsetSeq, newX, newY, nullptr);
        }
    }
}

void bfs(BmpImg& img) {
    static double constexpr THRESHOLD = 3000;
    vvbool been(img.height, vector<bool>(img.width, false));

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
                        if (colorDist(img.pixels[curY][curX], img.pixels[newY][newX]) < THRESHOLD) {
                            d.push_back({newX, newY});
                            img.pixels[newY][newX] = img.pixels[curY][curX];
                        }
                    }
                }
            }
        }
    }
}

void mix_2kp1(BmpImg& img, unsigned int k) { // i.e. mix_5 for k=2
    // static double constexpr THRESHOLD = 3000;
    unsigned int n = 2 * k + 1;
    vector<int> permut;
    for (uint i = 0; i < n * n; i++)
        permut.push_back(i);

    for (uint yc = k; yc < img.height - k; yc++) {
        // cout << yc << endl;
        for (uint xc = k; xc < img.width - k; xc++) {

            random_shuffle(permut.begin(), permut.end());
            for (uint oldind = 0; oldind < n * n; oldind++) {
                int newind = permut[oldind];

                int oldx_ = oldind % n;
                int oldy_ = oldind / n;

                int newx_ = newind % n;
                int newy_ = newind / n;

                int oldX = xc - k + oldx_;
                int oldY = yc - k + oldy_;

                int newX = xc - k + newx_;
                int newY = yc - k + newy_;

                swap(img.pixels[oldY][oldX], img.pixels[newY][newX]);
            }
        }
    }
}

Pixel averagePixel(vector<Pixel> pixels) {
    int n = pixels.size();
    int sumB = 0;
    int sumG = 0;
    int sumR = 0;
    for (const Pixel& p : pixels) {
        sumB += p.B;
        sumG += p.G;
        sumR += p.R;
    }
    return Pixel(sumB / n, sumG / n, sumR / n);
}

void avg_4adjM(BmpImg& img) {
    vector<vector<Pixel>> newPixels = img.pixels;
    vector<pair<int, int>> offSeq = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            int sumB = 0;
            int sumG = 0;
            int sumR = 0;
            double cnt = 0;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, img.width, img.height)) {
                    auto [curB, curG, curR] = img.pixels[curY][curX];
                    auto [B, G, R] = img.pixels[y][x];
                    sumB += curB + B;
                    sumG += curG + G;
                    sumR += curR + R;
                    cnt += 2;
                }
            }
            assert(cnt > 0);
            newPixels[y][x] = Pixel(sumB / cnt, sumG / cnt, sumR / cnt);
        }
    }
    img.pixels = newPixels;
}

vector<vector<Pixel>> avg_adjK(vector<vector<Pixel>> pixels, int height, int width, int k) {
    vector<vector<Pixel>> newPixels = pixels;
    vector<pair<int, int>> offSeq = genOffsetSeqSquareN(k);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto [B, G, R] = pixels[y][x];
            int sumB = B;
            int sumG = G;
            int sumR = R;
            double cnt = 1;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, width, height)) {
                    auto [curB, curG, curR] = pixels[curY][curX];
                    sumB += curB;
                    sumG += curG;
                    sumR += curR;
                    cnt++;
                }
            }
            newPixels[y][x] = Pixel(sumB / cnt, sumG / cnt, sumR / cnt);
        }
    }
    return newPixels;
}

void avg_adjK(BmpImg& img, int k) { img.pixels = avg_adjK(img.pixels, img.height, img.width, k); }

void ssaa4x(BmpImg& img) {
    vector<vector<Pixel>> buffer(2 * img.height, vector<Pixel>(2 * img.width, {0, 0, 0}));

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            buffer[2 * y][2 * x] = img[y][x];
            buffer[2 * y][2 * x + 1] = img[y][x];
            buffer[2 * y + 1][2 * x] = img[y][x];
            buffer[2 * y + 1][2 * y + 1] = img[y][x];
        }
    }

    avg_adjK(buffer, 2 * img.height, 2 * img.width, 31);

    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            img[y][x] = averagePixel({buffer[2 * y][2 * x], buffer[2 * y + 1][2 * x],
                                      buffer[2 * y][2 * x + 1], buffer[2 * y + 1][2 * x + 1]});
        }
    }
}

void avg_ifth(BmpImg& img, int TH, int k = 3) {
    vector<vector<Pixel>> newPixels = img.pixels;
    vector<pair<int, int>> offSeq = genOffsetSeqSquareN(k);
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            vector<Pixel> adj;
            for (auto [xoffset, yoffset] : offSeq) {
                int curX = x + xoffset;
                int curY = y + yoffset;
                if (inBounds(curX, curY, img.width, img.height))
                    adj.push_back(img[curY][curX]);
            }
            Pixel avg = averagePixel(adj);
            if (colorDist(img[y][x], avg) > TH)
                newPixels[y][x] = avg;
        }
    }
    img.pixels = newPixels;
}

struct Sigmoid {
    unsigned int limit_;
    double base_ = 2.7;
    Sigmoid(unsigned int limit)
            : limit_(limit){};
    Sigmoid(unsigned int limit, double base)
            : limit_(limit)
            , base_(base){};
    double operator()(double x) const { return 1.0 / (1 + pow(base_, -(x - limit_ / 2))); }
};

void bounds_avgsigm(BmpImg& img, uint TH, int n = 5) {
    // Sigmoid sigm({THRESHOLD});
    vector<pair<int, int>> offset = genOffsetSeqSquareN(n);

    vector<vector<Pixel>> new_pixels = img.pixels;
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            const Pixel& px = img[y][x];
            unsigned int sumDiffSq = 0;
            double cnt = 0;
            for (auto [xoff, yoff] : offset) {
                int cur_x = x + xoff;
                int cur_y = y + yoff;
                if (inBounds(cur_x, cur_y, img.width, img.height)) {
                    const Pixel& curpx = img[cur_y][cur_x];
                    sumDiffSq += (curpx.B - px.B) * (curpx.B - px.B) +
                                 (curpx.G - px.G) * (curpx.G - px.G) +
                                 (curpx.R - px.R) * (curpx.R - px.R);
                    cnt++;
                }
            }
            // unsigned char new_value = sigm(sumDiffSq / cnt) * 256;
            unsigned char new_value = 0;
            if (sumDiffSq < TH)
                new_value = 255;
            // if (y == x && x % 30 == 0)
            //     cout << sumDiffSq << ' ' << th << ' ' << (unsigned int)new_value << endl;

            new_pixels[y][x] = new_value;
        }
    }
    img.pixels = new_pixels;
}

void zakaz1(BmpImg& img) {
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            Pixel& p = img[y][x];
            if ((p.B + p.G + p.R) / 3 < 127)
                p = 0;
            else
                p = 255;
        }
    }
}

void zakaz2(BmpImg& img) {
    unsigned long long sumB = 0;
    unsigned long long sumG = 0;
    unsigned long long sumR = 0;
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            sumB += img[y][x].B;
            sumG += img[y][x].G;
            sumR += img[y][x].R;
        }
    }
    unsigned char avgB = sumB / (img.height * img.width);
    unsigned char avgG = sumG / (img.height * img.width);
    unsigned char avgR = sumR / (img.height * img.width);
    for (uint y = 0; y < img.height; y++) {
        for (uint x = 0; x < img.width; x++) {
            Pixel& p = img[y][x];
            if (colorDist(p, {0, 0, 0}) < colorDist({avgB, avgG, avgR}, {0, 0, 0}))
                p = 0;
            else
                p = 255;
        }
    }
}

void merge(BmpImg& img1, BmpImg& img2, BmpImg& img3) {
    assert(img1.height == img2.height && img2.height == img3.height);
    assert(img1.width == img2.width && img2.width == img3.width);

    for (uint y = 0; y < img1.height; y++) {
        for (uint x = 0; x < img1.width; x++) {
            // if (rand() % 2)
            //     img3[y][x] = img1[y][x];
            // else
            //     img3[y][x] = img2[y][x];
            img3[y][x] = averagePixel({img1[y][x], img2[y][x]});
        }
    }
}

void polar_addCoord(BmpImg& img, double dRad, double dZen, double dAzi) {
    for (uint yc = 0; yc < img.height; yc++) {
        for (uint xc = 0; xc < img.width; xc++) {
            Pixel& p = img[yc][xc];
            auto [rad, zen, azi] = toPolar(p.R, p.G, p.B);
            auto [x, y, z] = fromPolar(rad + dRad, zen + dZen, azi + dAzi);
            if (xc == yc && xc % 25 == 0) {
                cout << "rgb: " << (int)p.R << ' ' << (int)p.G << ' ' << (int)p.B << '\n';
                cout << "rza: " << rad << ' ' << zen << ' ' << azi << '\n';
                cout << "xyz: " << x << ' ' << y << ' ' << z << '\n';
            }
            p.R = x;
            p.G = y;
            p.B = z;
        }
    }
}

void polar_mulCoord(BmpImg& img, double dRad, double dZen, double dAzi) {
    for (uint yc = 0; yc < img.height; yc++) {
        for (uint xc = 0; xc < img.width; xc++) {
            Pixel& p = img[yc][xc];
            auto [rad, zen, azi] = toPolar(p.R, p.G, p.B);
            auto [x, y, z] = fromPolar(rad * dRad, zen * dZen, azi * dAzi);
            if (xc == yc && xc % 25 == 0) {
                cout << "rgb: " << (int)p.R << ' ' << (int)p.G << ' ' << (int)p.B << '\n';
                cout << "rza: " << rad << ' ' << zen << ' ' << azi << '\n';
                cout << "xyz: " << x << ' ' << y << ' ' << z << '\n';
            }
            p.R = x;
            p.G = y;
            p.B = z;
        }
    }
}

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