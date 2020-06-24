#ifndef bmp_h_included
#define bmp_h_included

typedef struct {
    unsigned char* header;
    unsigned char** pixels;
} BmpImage;

static constexpr unsigned int HEADER_LENGTH = 54;
static constexpr unsigned int BYTES_PER_PIXEL = 3;
static constexpr unsigned int BYTEPOS_RASTER_TOTAL_LENGTH = 34;
static constexpr unsigned int BYTEPOS_RASTER_WIDTH = 18;
static constexpr unsigned int BYTEPOS_RASTER_HEIGHT = 22;
static constexpr unsigned int BYTEPOS_FILE_SIZE = 2;

static constexpr int RV_SUCCESS = 0;
static constexpr int RV_FILE_ERROR = 1;
static constexpr int RV_IO_ERROR = 2;
static constexpr int RV_MEMORY_ERROR = 3;
static constexpr int RV_INVALID_ARGS = 4;

unsigned int get_raster_length(const unsigned char* header);
void set_raster_length(unsigned char* header, unsigned int length);

unsigned int get_raster_width(const unsigned char* header);
void set_raster_width(unsigned char* header, unsigned int width);

unsigned int get_raster_height(const unsigned char* header);
void set_raster_height(unsigned char* header, unsigned int height);

void reset_bmp_image(BmpImage* img);
void flush_bmp_image(BmpImage* img);

int load_bmp(const char* input_file, BmpImage* image);

int crop(BmpImage* image, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
int rotate(BmpImage* image);

int save_bmp(BmpImage* image, const char* output_file);

#endif