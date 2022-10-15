#ifndef PNGWRITER_H__
#define PNGWRITER_H__

#ifdef __cplusplus
	extern "C" {
#endif

int write_png_image(const char *filename, unsigned char *pixels, int w, int h, int has_alpha);

#ifdef __cplusplus
	}
#endif

#endif
