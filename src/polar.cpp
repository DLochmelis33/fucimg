#include "polar.hpp"

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

void polar_addCoord(BmpImg& img, double dRad, double dZen, double dAzi) {
    for (uint yc = 0; yc < img.height; yc++) {
        for (uint xc = 0; xc < img.width; xc++) {
            Pixel& p = img[yc][xc];
            auto [rad, zen, azi] = toPolar(p.R, p.G, p.B);
            auto [x, y, z] = fromPolar(rad + dRad, zen + dZen, azi + dAzi);
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
            p.R = x;
            p.G = y;
            p.B = z;
        }
    }
}
