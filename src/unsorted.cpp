#include "unsorted.hpp"
#include "offSeq.hpp"
#include "shapes.hpp"
#include <random>

void zakaz1(vvpix& pixels) {
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            Pixel& p = pixels[y][x];
            if ((p.B + p.G + p.R) / 3 < 127)
                p = 0;
            else
                p = 255;
        }
    }
}

void zakaz2(vvpix& pixels) {
    unsigned long long sumB = 0;
    unsigned long long sumG = 0;
    unsigned long long sumR = 0;
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            sumB += pixels[y][x].B;
            sumG += pixels[y][x].G;
            sumR += pixels[y][x].R;
        }
    }
    unsigned char avgB = sumB / (pixels.height * pixels.width);
    unsigned char avgG = sumG / (pixels.height * pixels.width);
    unsigned char avgR = sumR / (pixels.height * pixels.width);
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            Pixel& p = pixels[y][x];
            if (colorDist(p, {0, 0, 0}) < colorDist({avgB, avgG, avgR}, {0, 0, 0}))
                p = 0;
            else
                p = 255;
        }
    }
}

void bounds_avgsigm(vvpix& pixels, uint TH, int n) {
    // Sigmoid sigm({THRESHOLD});
    vector<pair<int, int>> offset = osSquare(n, false);

    vvpix new_pixels = pixels;
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            const Pixel& px = pixels[y][x];
            unsigned int sumDiffSq = 0;
            double cnt = 0;
            for (auto [xoff, yoff] : offset) {
                int cur_x = x + xoff;
                int cur_y = y + yoff;
                if (inBounds(cur_x, cur_y, pixels.width, pixels.height)) {
                    const Pixel& curpx = pixels[cur_y][cur_x];
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
            //     cout << sumDiffSq << ' ' << th << ' ' << (unsigned
            //     int)new_value << endl;

            new_pixels[y][x] = new_value;
        }
    }
    pixels = new_pixels;
}

void mix_2kp1(vvpix& pixels, unsigned int k) { // i.e. mix_5 for k=2
    // static double constexpr THRESHOLD = 3000;
    unsigned int n = 2 * k + 1;
    vector<int> permut;
    for (uint i = 0; i < n * n; i++)
        permut.push_back(i);

    for (uint yc = k; yc < pixels.height - k; yc++) {
        // cout << yc << endl;
        for (uint xc = k; xc < pixels.width - k; xc++) {

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

                std::swap(pixels[oldY][oldX], pixels[newY][newX]);
            }
        }
    }
}

void color_k_means(vvpix& pixels, int k, double stopSensitivity, int stepsLimit) {
    vvpix newPixels = pixels;

    vector<Pixel> pxs;
    for (uint y = 0; y < newPixels.height; y++)
        for (uint x = 0; x < newPixels.width; x++)
            pxs.push_back(newPixels[y][x]);

    std::cout << "k-means started...\n";
    auto kmeansStartTime = std::chrono::steady_clock::now();

    // generate initial distribution
    vector<Pixel> centers;
    for (int i = 0; i < k; i++) {
        uint x = rand() % pixels.width;
        uint y = rand() % pixels.height;
        centers.push_back(pixels[y][x]);
    }
    vector<vector<pair<int, int>>> clusters(k); // save pixel coords

    auto updateClusters = [&]() {
        for(vector<pair<int, int>> cluster : clusters)
            cluster.clear();
        for (uint y = 0; y < pixels.height; y++) {
            for (uint x = 0; x < pixels.width; x++) {
                uint closestColorCenterInd = 0;
                double closestColorDist = colorDist(pixels[y][x], centers[0]);
                for (int i = 1; i < k; i++) {
                    double curColorDist = colorDist(pixels[y][x], centers[i]);
                    if (curColorDist < closestColorDist) {
                        closestColorDist = curColorDist;
                        closestColorCenterInd = i;
                    }
                }
                clusters[closestColorCenterInd].push_back({x, y});
            }
        }
    };
    updateClusters();
    // iterations
    for (int step = 0; step < stepsLimit; step++) { // avoid while(true)
        // calculate new centers
        vector<Pixel> newCenters(k);
        bool anyChanges = false;
        for (int i = 0; i < k; i++) {
            vector<Pixel> curCluster;
            for (auto [x, y] : clusters[i])
                curCluster.push_back(pixels[y][x]);
            Pixel newCenter = averagePixel(curCluster);
            if (colorDist(newCenter, centers[i]) > stopSensitivity)
                anyChanges = true;
            newCenters[i] = newCenter;
        }
        if (!anyChanges)
            break;
        // update clusters with new centers
        updateClusters();
    }

    auto kmeansEndTime = std::chrono::steady_clock::now();
    std::cout
        << "k-means ended in "
        << std::chrono::duration<double, std::milli>(kmeansEndTime - kmeansStartTime).count() /
               1000.0
        << " seconds\n";

    for (uint y = 0; y < newPixels.height; y++) {
        for (uint x = 0; x < newPixels.width; x++) {
            double closestColorDist = colorDist(pixels[y][x], centers[0]);
            uint closestColorCenterInd = 0;
            for(int i = 0; i < k; i++){
                double curColorDist = colorDist(pixels[y][x], centers[i]);
                if(curColorDist < closestColorDist){
                    closestColorDist = curColorDist;
                    closestColorCenterInd = i;
                }
            }
            newPixels[y][x] = centers[closestColorCenterInd];
        }
    }
    pixels = newPixels;
}

std::vector<Circle> generateCircles(int n, double lam, uint w, uint h, double radScale) {
    std::mt19937 gen(rand());

    std::exponential_distribution<double> d_rad(lam);
    std::uniform_int_distribution<int> d_x(0, w), d_y(0, h);

    std::vector<Circle> result;
    for(int i = 0; i < n; ++i)
        result.push_back({(d_rad(gen) * radScale), d_x(gen), d_y(gen)});

    return result;
}

void circlify(vvpix& pixels, int n, double lam, double radScale, double minRad, uint shuffleCoef){
    vvpix new_pixels = pixels;

    vector<Circle> circles = generateCircles(n, lam, pixels.width, pixels.height, radScale);

    std::sort(circles.begin(), circles.end(), 
        [&](const Circle& c1, const Circle& c2) {
            return c1.rad > c2.rad + (rand() % std::max((uint) 1, shuffleCoef));
        });

    for(auto [rad, xc, yc] : circles){
        // std::cout << rad << ' ' << xc << ' ' << yc << std::endl;
        if(rad < minRad)
            continue;
        Pixel p = median_os(pixels, osCircle(rad, false), xc, yc);
        drawCircle(new_pixels, rad, xc, yc, p);
    }

    pixels = new_pixels;
}
