#include <stdio.h>
#if ((__STDC_VERSION__ >= 199901L) || (_MSC_VER))
	#include <stdint.h>
#endif
#include <string.h>
#include <errno.h>
#include "open-simplex-noise.h"
#include "pngwriter.h"

#define WIDTH 800
#define HEIGHT 600
#define FEATURE_SIZE 24

#define SINGLE_OCTAVE

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
	int x, y;
	double value;
	double v0, v1, v2; /* values from different octaves. */
	uint32_t rgb;
	uint32_t image2d[HEIGHT][WIDTH];
	uint32_t image3d[HEIGHT][WIDTH];
	uint32_t image4d[HEIGHT][WIDTH];
	struct osn_context *ctx;

	open_simplex_noise(77374, &ctx);

	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
#if defined(SINGLE_OCTAVE)
			value = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE, (double) y / FEATURE_SIZE, 0.0, 0.0);
#else
			/* Use three octaves: frequency N, N/2 and N/4 with relative amplitudes 4:2:1. */
			v0 = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE / 4,
						(double) y / FEATURE_SIZE / 4, 0.0, 0.0);
			v1 = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE / 2,
						(double) y / FEATURE_SIZE / 2, 0.0, 0.0);
			v2 = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE / 1,
						(double) y / FEATURE_SIZE / 1, 0.0, 0.0);
			value = v0 * 4 / 7.0 + v1 * 2 / 7.0 + v2 * 1 / 7.0;
#endif
			rgb = 0x010101 * (uint32_t) ((value + 1) * 127.5);
			image2d[y][x] = ((uint32_t) 0x0ff << 24) | (rgb);

			value = open_simplex_noise2(ctx, (double) x / FEATURE_SIZE, (double) y / FEATURE_SIZE);
			rgb = 0x010101 * (uint32_t) ((value + 1) * 127.5);
			image3d[y][x] = ((uint32_t) 0x0ff << 24) | (rgb);

			value = open_simplex_noise3(ctx, (double) x / FEATURE_SIZE, (double) y / FEATURE_SIZE, 0.0);
			rgb = 0x010101 * (uint32_t) ((value + 1) * 127.5);
			image4d[y][x] = ((uint32_t) 0x0ff << 24) | (rgb);
		}
	}

	write_png_image("out/test1-2d.png", (unsigned char *) image2d, WIDTH, HEIGHT, 1);
	write_png_image("out/test1-3d.png", (unsigned char *) image3d, WIDTH, HEIGHT, 1);
	write_png_image("out/test1-4d.png", (unsigned char *) image4d, WIDTH, HEIGHT, 1);

	open_simplex_noise_free(ctx);

	return 0;
}
