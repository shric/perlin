// From http://www.labbookpages.co.uk/software/imgProc/libPNG.html
// with minor adjustment (allows compression level to be set).

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>

#include "write_png.h"

int writeImage(const char* filename, unsigned long width, unsigned long height,
               unsigned char *buffer, char* title, int compression)
{
	int code = 0;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	// One row of 8 bit RGB pixels
	png_bytep row = malloc(3 * width * sizeof(png_byte));

	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto end;
	}

	if (NULL == (png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                 NULL, NULL, NULL))) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto end;
	}

  png_set_compression_level(png_ptr, compression);

	if (NULL == (info_ptr = png_create_info_struct(png_ptr))) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto end;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto end;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Write image data
	unsigned long x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
      row[x * 3 + 0] = buffer[0];
      row[x * 3 + 1] = buffer[1];
      row[x * 3 + 2] = buffer[2];
      buffer += 3;
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	end:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	free(row);
	return code;
}
