#include "common.hpp"
#include <functional>

using colorfunc = std::function<double(double, double)>;

void apply_color_func(vvpix& img, colorfunc fr, colorfunc fg, colorfunc fb, double wscale, double hscale);

