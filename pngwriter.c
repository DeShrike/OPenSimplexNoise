#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <png.h>

#include "pngwriter.h"

int write_png_image(const char *filename, unsigned char *pixels, int w, int h, int has_alpha)
{
    printf("Saving file %s\n", filename);

	png_structp png_ptr;
	png_infop info_ptr;
	png_byte **row;
	int x, y, rc, colordepth = 8;
	int bytes_per_pixel = has_alpha ? 4 : 3;
	FILE *f;

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "fopen: %s:%s\n", filename, strerror(errno));
		return -1;
	}

	rc = -1;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
    	goto cleanup1;
    }
    
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
    {
		goto cleanup2;
    }

	if (setjmp(png_jmpbuf(png_ptr))) /* oh libpng, you're old as dirt, aren't you. */
	{
    	goto cleanup2;
    }

	png_set_IHDR(png_ptr, info_ptr, (size_t) w, (size_t) h, colordepth,
			has_alpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);

	row = png_malloc(png_ptr, h * sizeof(*row));
	for (y = 0; y < h; y++)
	{
		row[y] = png_malloc(png_ptr, w * bytes_per_pixel);
		for (x = 0; x < w; x++)
		{
			unsigned char *r = (unsigned char *) row[y];
			unsigned char *src = (unsigned char *) &pixels[y * w * bytes_per_pixel + x * bytes_per_pixel];
			unsigned char *dest = &r[x * bytes_per_pixel];
			memcpy(dest, src, bytes_per_pixel);
		}
	}

	png_init_io(png_ptr, f);
	png_set_rows(png_ptr, info_ptr, row);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING, NULL);

	for (y = 0; y < h; y++)
    {
		png_free(png_ptr, row[y]);
    }

	png_free(png_ptr, row);
	rc = 0;

cleanup2:
	png_destroy_write_struct(&png_ptr, &info_ptr);

cleanup1:
	fclose(f);

	return rc;
}
