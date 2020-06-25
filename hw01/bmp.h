#ifndef bmp_h_included
#define bmp_h_included

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		unsigned char *header;
		unsigned char **pixels;
	} BmpImage;

	extern const unsigned int HEADER_LENGTH;
	extern const unsigned int BYTES_PER_PIXEL;
	extern const unsigned int BYTEPOS_RASTER_TOTAL_LENGTH;
	extern const unsigned int BYTEPOS_RASTER_WIDTH;
	extern const unsigned int BYTEPOS_RASTER_HEIGHT;
	extern const unsigned int BYTEPOS_FILE_SIZE;

	// RV stands for Return Value
	extern const char *RV_TYPES[];
	extern const unsigned int RV_TYPES_AMOUNT;
	extern const int RV_SUCCESS;
	extern const int RV_FILE_ERROR;
	extern const int RV_IO_ERROR;
	extern const int RV_MEMORY_ERROR;
	extern const int RV_INVALID_ARGS;

	unsigned int get_raster_length(const unsigned char *header);
	void set_raster_length(unsigned char *header, unsigned int length);

	unsigned int get_raster_width(const unsigned char *header);
	void set_raster_width(unsigned char *header, unsigned int width);

	unsigned int get_raster_height(const unsigned char *header);
	void set_raster_height(unsigned char *header, unsigned int height);

	void reset_bmp_image(BmpImage *img);
	void flush_bmp_image(BmpImage *img);

	int load_bmp(const char *input_file, BmpImage *image);

	int crop(BmpImage *image, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	int rotate(BmpImage *image);

	int save_bmp(BmpImage *image, const char *output_file);

#ifdef __cplusplus
}
#endif

#endif