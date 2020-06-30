#include "offSeq.hpp"

OffSeq osCircle(double rad, bool outline, double width) {
    OffSeq res;
    for (int xoffset = -rad - width; xoffset <= rad + width; xoffset++) {
        for (int yoffset = -rad - width; yoffset <= rad + width; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            double distance = dist({xoffset, yoffset}, {0, 0});
            if ((outline && (abs(distance - rad) < width)) ||
                (!outline && (distance < rad + width)))
                res.push_back({xoffset, yoffset});
        }
    }
    return res;
}

OffSeq osSquare(int sqrad, bool outline) { // sqrad = side of square / 2
    OffSeq res;
    for (int xoffset = -sqrad; xoffset <= sqrad; xoffset++) {
        for (int yoffset = -sqrad; yoffset <= sqrad; yoffset++) {
            if (xoffset == 0 && yoffset == 0)
                continue;
            if (outline &&
                !(xoffset == -sqrad || xoffset == sqrad || yoffset == -sqrad || yoffset == sqrad))
                continue;
            res.push_back({xoffset, yoffset});
        }
    }
    return res;
}

OffSeq osGaussian(int count, double sigma) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution dist_distribution{0.0, sigma};
    std::uniform_real_distribution angle_distribution{0.0, 2 * M_PI}; // 1 * PI is also fine

    OffSeq res;
    for (int i = 0; i < count; i++) {
        double dist = dist_distribution(gen);
        double angle = angle_distribution(gen);
        int xoff = dist * sin(angle);
        int yoff = dist * cos(angle);
        res.push_back({xoff, yoff});
    }
    return res;
}

OffSeq osAdjacent() { return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; }