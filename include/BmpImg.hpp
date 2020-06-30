#pragma once

#include "bmp.h"

#include <cassert>
#include <string>
#include <vector>

class BmpImg {
    public:

    struct Pixel {
        unsigned char B, G, R;

        Pixel(unsigned char b = 0, unsigned char g = 0, unsigned char r = 0) : B{b}, G{g}, R{r} {}

        Pixel& operator=(unsigned char val) {
            B = G = R = val;
            return *this;
        }

        Pixel operator+(const Pixel& p) const {
            return {static_cast<unsigned char>(B + p.B), static_cast<unsigned char>(G + p.G),
                    static_cast<unsigned char>(R + p.R)};
        }
        Pixel& operator+=(const Pixel& p) {
            B += p.B;
            G += p.G;
            R += p.R;
            return *this;
        }
        Pixel& operator+=(unsigned char val) {
            B += val;
            G += val;
            R += val;
            return *this;
        }
        Pixel operator*(double x) const {
            return {static_cast<unsigned char>(B * x), static_cast<unsigned char>(G * x),
                    static_cast<unsigned char>(R * x)};
        }
        Pixel operator/(double x) const {
            return {static_cast<unsigned char>(B / x), static_cast<unsigned char>(G / x),
                    static_cast<unsigned char>(R / x)};
        }

        int sumSq() const { return B * B + G * G + R * R; }
    };

    class vvpix {
      private:
        std::vector<std::vector<Pixel>> pixels;

      public:
        uint height, width;
        vvpix(const std::vector<std::vector<Pixel>>& pixels_) : pixels(pixels_) {
            height = pixels.size();
            assert(height > 0);
            width = pixels[0].size();
        }
        vvpix() : pixels{}, height(), width() {}
        std::vector<Pixel>& operator[](size_t y) { return pixels[y]; }
    };

    BmpImg(const std::string& file) : filename(file) {
        reset_bmp_image(&image);
        load_bmp(filename.c_str(), &image);
        height = get_raster_height(image.header);
        width = get_raster_width(image.header);
        pixels = std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));
        assert(BYTES_PER_PIXEL == 3);
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++)
                pixels[y][x] = {image.pixels[y][3 * x + 0], image.pixels[y][3 * x + 1],
                                image.pixels[y][3 * x + 2]};
    }

    BmpImg(const BmpImg& other) = delete;
    BmpImg& operator=(const BmpImg& other) = delete;

    std::vector<Pixel>& operator[](int y) { return pixels[y]; }

    ~BmpImg() {
        assert(BYTES_PER_PIXEL == 3);
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) {
                image.pixels[y][3 * x + 0] = pixels[y][x].B;
                image.pixels[y][3 * x + 1] = pixels[y][x].G;
                image.pixels[y][3 * x + 2] = pixels[y][x].R;
            }
        save_bmp(&image, ("MOD_" + filename).c_str());
        flush_bmp_image(&image);
    }

    operator vvpix& () { return pixels; }

private:
    BmpImage image;
public:
    vvpix pixels;
    size_t height;
    size_t width;
    std::string filename;
};