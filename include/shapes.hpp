#pragma once
#include "common.hpp"

using Point = pair<int, int>;

void drawCircle(vvpix& pixels, uint rad, int xc, int yc, Pixel color);

void drawSquare(vvpix& pixels, int sqRad, int xc, int yc, Pixel color);

void drawRectangle(vvpix& pixels, Point p1, Point p2, Pixel color);

Point rotate(Point p, double angle); // relative to (0, 0)

void drawRectangle(vvpix& pixels, int xc, int yc, uint w, uint h, double angle, Pixel color);

void drawSegment(vvpix& pixels, Point p1, Point p2, double width, Pixel color);