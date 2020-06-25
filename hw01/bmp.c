#include "bmp.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int HEADER_LENGTH = 54;
const unsigned int BYTES_PER_PIXEL = 3;
const unsigned int BYTEPOS_RASTER_TOTAL_LENGTH = 34;
const unsigned int BYTEPOS_RASTER_WIDTH = 18;
const unsigned int BYTEPOS_RASTER_HEIGHT = 22;
const unsigned int BYTEPOS_FILE_SIZE = 2;

const int RV_SUCCESS = 0;
const int RV_FILE_ERROR = 1;
const int RV_IO_ERROR = 2;
const int RV_MEMORY_ERROR = 3;
const int RV_INVALID_ARGS = 4;

const unsigned int RV_TYPES_AMOUNT = 5;
const char* RV_TYPES[] = {
	"success", 
	"file error", 
	"io error", 
	"memory error", 
	"invalid arguments"
};

// Internal functions

unsigned int get_padding_size(unsigned int width) {
    return (4 - ((width * BYTES_PER_PIXEL) % 4)) % 4;
}

unsigned int get_padded_width(unsigned int width) {
    return width * BYTES_PER_PIXEL + get_padding_size(width);
}

unsigned int read_n_bytes(const unsigned char* first_byte, unsigned int n) {
    assert(n <= 4);
    unsigned int ans = 0;
    for (unsigned int i = 0; i < n; i++) {
        ans += ((unsigned int)(first_byte[i])) << (8 * i);
    }
    return ans;
}

void write_x_to_bytes(unsigned char* first_byte, unsigned int x, unsigned int bytes_count) {
    for (unsigned int i = 0; i < bytes_count; i++) {
        first_byte[i] = (unsigned char)(x >> (i * 8));
    }
}

// Define getters and setters from header

unsigned int get_raster_length(const unsigned char* header) {
    assert(header);
    return read_n_bytes(header + BYTEPOS_RASTER_TOTAL_LENGTH, 4);
}

unsigned int get_raster_width(const unsigned char* header) {
    assert(header);
    return read_n_bytes(header + BYTEPOS_RASTER_WIDTH, 4);
}

unsigned int get_raster_height(const unsigned char* header) {
    assert(header);
    return read_n_bytes(header + BYTEPOS_RASTER_HEIGHT, 4);
}

unsigned int get_file_size(const unsigned char* header) {
    assert(header);
    return read_n_bytes(header + BYTEPOS_FILE_SIZE, 4);
}

void set_raster_length(unsigned char* header, unsigned int length) {
    assert(header);
    write_x_to_bytes(header + BYTEPOS_RASTER_TOTAL_LENGTH, length, 4);
}

void set_raster_width(unsigned char* header, unsigned int width) {
    assert(header);
    write_x_to_bytes(header + BYTEPOS_RASTER_WIDTH, width, 4);
}

void set_raster_height(unsigned char* header, unsigned int height) {
    assert(header);
    write_x_to_bytes(header + BYTEPOS_RASTER_HEIGHT, height, 4);
}

void set_file_size(unsigned char* header, unsigned int file_size) {
    assert(header);
    write_x_to_bytes(header + BYTEPOS_FILE_SIZE, file_size, 4);
}

void update_header_data(unsigned char* header, unsigned int width, unsigned int height,
                        unsigned int length) {
    set_raster_width(header, width);
    set_raster_height(header, height);
    set_raster_length(header, length);
    set_file_size(header, HEADER_LENGTH + length);
}

// Define other .h functions

void reset_bmp_image(BmpImage* img) {
    // may cause memory leak if used improperly
    img->header = NULL;
    img->pixels = NULL;
}

void flush_bmp_image(BmpImage* img) {
    assert(img->header && img->pixels);

    unsigned int height = get_raster_height(img->header);
    for (unsigned int i = 0; i < height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img->header);

    reset_bmp_image(img);
}

// main functions

int load_bmp(const char* input_file, BmpImage* image) {
    assert(image);
    // image data will be overwritten; assertion prevents memory leak
    assert(image->header == NULL && image->pixels == NULL);

    image->header = (unsigned char*)malloc(HEADER_LENGTH * sizeof(unsigned char));
    if (image->header == NULL)
        return RV_MEMORY_ERROR;

    FILE* fin = fopen(input_file, "rb");
    if (fin == NULL)
        return RV_FILE_ERROR;
    unsigned int read_count = fread(image->header, sizeof(unsigned char), HEADER_LENGTH, fin);
    if (read_count != HEADER_LENGTH)
        return RV_IO_ERROR;

    unsigned int height = get_raster_height(image->header);
    unsigned int width = get_raster_width(image->header);
    unsigned int padded_bytes_width = get_padded_width(width);

    image->pixels = (unsigned char**)malloc(height * padded_bytes_width * sizeof(unsigned char*));
    if (image->pixels == NULL)
        return RV_MEMORY_ERROR;
    for (unsigned int i = 0; i < height; i++) {
        image->pixels[i] = (unsigned char*)malloc(padded_bytes_width * sizeof(unsigned char));
        if (image->pixels[i] == NULL)
            return RV_MEMORY_ERROR;
    }

    unsigned int padding_size = get_padding_size(width);
    unsigned int width_bytes = width * BYTES_PER_PIXEL;

    for (unsigned int i = 0; i < height; i++) {
        unsigned int read_count = fread(image->pixels[i], sizeof(unsigned char), width_bytes, fin);
        if (read_count != width_bytes)
            return RV_IO_ERROR;
        for (unsigned int j = 0; j < padding_size; j++) {
            image->pixels[i][width_bytes + j] = 0;
            int seek_res = fseek(fin, 1, SEEK_CUR);
            if (seek_res != 0)
                return RV_IO_ERROR;
        }
    }

    fclose(fin);

    return RV_SUCCESS;
}

int crop(BmpImage* img, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    assert(img->header && img->pixels);

    unsigned int width = get_raster_width(img->header);
    unsigned int height =
        get_raster_height(img->header); // task was updated, now only positive height

    if (w == 0 || h == 0 || x > width || y > height || (x + w) > width || (y + h) > height ||
        w > INT_MAX || h > INT_MAX)
        return RV_INVALID_ARGS;

    unsigned int w_bytes = w * BYTES_PER_PIXEL;
    unsigned int padded_w = get_padded_width(w);

    // allocate new raster
    unsigned char** new_pixels = (unsigned char**)malloc(h * sizeof(unsigned char*));
    if (new_pixels == NULL)
        return RV_MEMORY_ERROR;
    for (unsigned int i = 0; i < h; i++) {
        new_pixels[i] = (unsigned char*)malloc(padded_w * sizeof(unsigned char));
        if (new_pixels[i] == NULL)
            return RV_MEMORY_ERROR;
    }

    unsigned int x_bytes = x * BYTES_PER_PIXEL;

    // convert to old file coordinates
    unsigned int start_y = height - y - h;
    unsigned int finish_y = start_y + h;

    unsigned int start_x = x_bytes;
    unsigned int finish_x = start_x + w_bytes;

    // cropping image
    for (unsigned int i = start_y; i < finish_y; i++) {
        // copy pixels
        for (unsigned int j = start_x; j < finish_x; j++) {
            new_pixels[i - start_y][j - start_x] = img->pixels[i][j];
        }
        // add padding
        for (unsigned int j = 0; j < get_padding_size(w); j++) {
            new_pixels[i - start_y][w_bytes + j] = 0;
        }
    }

    // free old pixels and assign to new
    for (unsigned int i = 0; i < height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    img->pixels = new_pixels;

    update_header_data(img->header, w, h, padded_w);

    return RV_SUCCESS;
}

int rotate(BmpImage* img) {
    assert(img->header && img->pixels);

    unsigned int width = get_raster_width(img->header);
    unsigned int height = get_raster_height(img->header);
    //	unsigned int width_bytes = width * BYTES_PER_PIXEL;
    //	unsigned int padded_width = get_padded_width(width);

    unsigned int new_width = height;
    unsigned int new_width_bytes = new_width * BYTES_PER_PIXEL;
    unsigned int new_padded_width = get_padded_width(new_width);
    unsigned int new_height = width;

    // allocate new raster
    unsigned char** new_pixels = (unsigned char**)malloc(new_height * sizeof(unsigned char*));
    if (new_pixels == NULL)
        return RV_MEMORY_ERROR;
    for (unsigned int i = 0; i < new_height; i++) {
        new_pixels[i] = (unsigned char*)malloc(new_padded_width * sizeof(unsigned char));
        if (new_pixels[i] == NULL)
            return RV_MEMORY_ERROR;
    }

    unsigned int new_j; // for width as usual
    unsigned int new_i; // for height as usual

    for (unsigned int j = width - 1; j <= width - 1; j--) { // condition triggers when overflowing
        // copy new line
        for (unsigned int i = 0; i < height; i++) {
            // transform to new coordinates
            new_i = width - 1 - j;
            new_j = i;

            // copy single pixel
            new_pixels[new_i][BYTES_PER_PIXEL * new_j + 0] =
                img->pixels[i][BYTES_PER_PIXEL * j + 0];
            new_pixels[new_i][BYTES_PER_PIXEL * new_j + 1] =
                img->pixels[i][BYTES_PER_PIXEL * j + 1];
            new_pixels[new_i][BYTES_PER_PIXEL * new_j + 2] =
                img->pixels[i][BYTES_PER_PIXEL * j + 2];
        }
        for (unsigned int i = 0; i < get_padding_size(new_width); i++) {
            new_pixels[new_i][new_width_bytes + i] = 0;
        }
    }

    // free old pixels
    for (unsigned int i = 0; i < height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    img->pixels = new_pixels;

    update_header_data(img->header, new_width, new_height, new_height * new_padded_width);

    return RV_SUCCESS;
}

int save_bmp(BmpImage* img, const char* output_file_name) {
    assert(img->pixels);
    assert(output_file_name);
    assert(img->header && img->pixels && output_file_name);

    FILE* fout = fopen(output_file_name, "wb");
    if (fout == NULL)
        return RV_FILE_ERROR;

    unsigned int written_count = fwrite(img->header, 1, HEADER_LENGTH, fout);
    if (written_count != HEADER_LENGTH)
        return RV_IO_ERROR;

    unsigned int raster_height = get_raster_height(img->header);
    unsigned int raster_width = get_raster_width(img->header);
    unsigned int padded_bytes_width = get_padded_width(raster_width);

    for (unsigned int i = 0; i < raster_height; i++) {
        written_count = fwrite(img->pixels[i], 1, padded_bytes_width, fout);
        if (written_count != padded_bytes_width)
            return RV_IO_ERROR;
    }

    fclose(fout);

    return RV_SUCCESS;
}
