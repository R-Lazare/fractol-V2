#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SQRT_3 1.732

typedef struct {
    void *mlx_ptr;
    void *img_ptr;
    char *addr;
    void *win_ptr;
    int precision;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void draw_line(t_data *data, int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        my_mlx_pixel_put(data, x1, y1, color); // Use your custom function
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void fractale(t_data *data, int x1, int y1, int x2, int y2, int level, int precision) {
    if (level >= precision) {
        draw_line(data, x1, y1, x2, y2, 0xFFFFFF); // White color
        return;
    }

    int dx = x2 - x1, dy = y2 - y1;
    int x3 = x1 + dx / 3, y3 = y1 + dy / 3;
    int x4 = x1 + dx / 2 - dy / (2 * SQRT_3), y4 = y1 + dy / 2 + dx / (2 * SQRT_3);
    int x5 = x1 + 2 * dx / 3, y5 = y1 + 2 * dy / 3;

    fractale(data, x1, y1, x3, y3, level + 1, precision);
    fractale(data, x3, y3, x4, y4, level + 1, precision);
    fractale(data, x4, y4, x5, y5, level + 1, precision);
    fractale(data, x5, y5, x2, y2, level + 1, precision);
}

void draw_fractal(t_data *data) {
    // Destroy the old image
    if (data->img_ptr) {
        mlx_destroy_image(data->mlx_ptr, data->img_ptr);
    }

    // Create a new image
    data->img_ptr = mlx_new_image(data->mlx_ptr, 1100, 1100);
    data->addr = mlx_get_data_addr(data->img_ptr, &data->bits_per_pixel, &data->line_length, &data->endian);

    int height = 1100;
    int baseWidth = 1100;
    int sideLength = baseWidth / 2;
    int heightOffset = (height - (int)(sideLength * SQRT_3 / 2)) / 2;

    fractale(data, baseWidth / 4, heightOffset, 3 * baseWidth / 4, heightOffset, 0, data->precision);
    fractale(data, 3 * baseWidth / 4, heightOffset, baseWidth / 2, height - heightOffset, 0, data->precision);
    fractale(data, baseWidth / 2, height - heightOffset, baseWidth / 4, heightOffset, 0, data->precision);

    // Display the new image
    mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
}

int key_hook(int keycode, t_data *data) {
    printf("keycode: %d\n", keycode);
    if (keycode == 65307) {  // ESC key to quit (keycode might vary)
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);
        exit(0);
    }
    return 0;
}

int mouse_hook(int button, int x, int y, t_data *data) {
    if (button == 4) {  // Scroll up to increase precision
        data->precision++;
    } else if (button == 5) {  // Scroll down to decrease precision
        if (data->precision > 0) {
            data->precision--;
        }
    }
    draw_fractal(data);
    return 0;
}

int main(int argc, char **argv) {
    t_data *data;

    data = (t_data *)malloc(sizeof(t_data));
    if (!data) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    data->precision = (argc > 1) ? atoi(argv[1]) : 5;

    data->mlx_ptr = mlx_init();
    if (!data->mlx_ptr) {
        free(data);
        fprintf(stderr, "Failed to initialize mlx");
        return EXIT_FAILURE;
    }

    data->win_ptr = mlx_new_window(data->mlx_ptr, 1100, 1100, "Koch Snowflake");
    if (!data->win_ptr) {
        free(data);
        fprintf(stderr, "Failed to create window");
        return EXIT_FAILURE;
    }

    data->img_ptr = mlx_new_image(data->mlx_ptr, 1100, 1100);
    if (!data->img_ptr) {
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);
        free(data);
        fprintf(stderr, "Failed to create image");
        return EXIT_FAILURE;
    }

    data->addr = mlx_get_data_addr(data->img_ptr, &data->bits_per_pixel, &data->line_length, &data->endian);

    mlx_key_hook(data->win_ptr, key_hook, data); // Handle keyboard input
    mlx_mouse_hook(data->win_ptr, mouse_hook, data); // Handle mouse input

    // Draw each side of the snowflake
    int height = 1100;
    int baseWidth = 1100;
    int heightOffset = (height - (int)(baseWidth * SQRT_3 / 2)) / 2;

    draw_fractal(data);
    mlx_loop(data->mlx_ptr); // Start the event loop
    return 0;
}
