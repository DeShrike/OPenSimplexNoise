#include <stdio.h>
#if ((__STDC_VERSION__ >= 199901L) || (_MSC_VER))
	#include <stdint.h>
#endif
#include <string.h>
#include <errno.h>
#include "open-simplex-noise.h"
#include "pngwriter.h"

#define WIDTH 1024
#define HEIGHT 720
#define FEATURE_SIZE 100

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
	int x, y;
	double value;
	uint32_t r, g, b, rgb;
	uint32_t image2d[HEIGHT][WIDTH];

	struct osn_context *ctx;

	open_simplex_noise(1000, &ctx);

	double smallest,  biggest;
	smallest = 1000.0;
	biggest = -1000.0;

	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			value = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE + 0.1, (double) y / FEATURE_SIZE, 0.0, 0.0) + 1.0;
			r = (uint32_t) (value * 255.0 / 2.0);

			value = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE - 0.1, (double) y / FEATURE_SIZE, 0.0, 0.0) + 1.0;
			g = (uint32_t) (value * 255.0 / 2.0);

			value = open_simplex_noise4(ctx, (double) x / FEATURE_SIZE, (double) y / FEATURE_SIZE, 0.0, 0.0) + 1.0;
			b = (uint32_t) (value * 255.0 / 2.0);

			if (value > biggest)
			{
				biggest = value;
			}

			if (value < smallest)
			{
				smallest = value;
			}

			rgb = r | (g << 8) | (b << 16);
			image2d[y][x] = ((uint32_t) 0x0ff << 24) | (rgb);
		}
	}

	printf("Smallest: %f \n", smallest);
	printf("Biggest: %f \n", biggest);

	write_png_image("out/test2-2d.png", (unsigned char *) image2d, WIDTH, HEIGHT, 1);

	open_simplex_noise_free(ctx);

	return 0;
}
