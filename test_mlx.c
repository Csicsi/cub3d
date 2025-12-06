#include "MLX42/include/MLX42/MLX42.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

void draw_test(mlx_t* mlx, mlx_image_t* img)
{
	// Draw a simple gradient
	for (uint32_t y = 0; y < HEIGHT; y++)
	{
		for (uint32_t x = 0; x < WIDTH; x++)
		{
			uint32_t color = 0xFF000000 | (x * 255 / WIDTH) << 16 | (y * 255 / HEIGHT) << 8;
			mlx_put_pixel(img, x, y, color);
		}
	}
	printf("Test pattern drawn\n");
}

int main(void)
{
	mlx_t* mlx;
	mlx_image_t* img;

	printf("Initializing MLX42...\n");
	mlx = mlx_init(WIDTH, HEIGHT, "MLX42 Test", true);
	if (!mlx)
	{
		printf("Failed to initialize MLX42\n");
		return 1;
	}
	printf("MLX42 initialized successfully\n");

	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
	{
		printf("Failed to create image\n");
		mlx_terminate(mlx);
		return 1;
	}
	printf("Image created\n");

	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
	{
		printf("Failed to display image\n");
		mlx_terminate(mlx);
		return 1;
	}
	printf("Image displayed\n");

	draw_test(mlx, img);

	printf("Starting main loop...\n");
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return 0;
}
