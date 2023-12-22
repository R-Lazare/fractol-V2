/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:32:10 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/22 18:00:08 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

inline double	custom_fabs(double x)
{
	uint64_t	*ptr;

	ptr = (uint64_t *)&x;
	*ptr &= 0x7FFFFFFFFFFFFFFF;
	return (x);
}

void	calc_log(t_data *img)
{
	double	*logt;
	int		i;

	i = 0;
	logt = arena_alloc(img->arena, 6001 * sizeof(double));
	while (i < 6001)
	{
		logt[i] = 1 - log(log(i / 1000)) / img->cos;
		i++;
	}
	img->logt = logt;
}

static inline t_c	sum_i(t_c a, t_c b)
{
	return ((t_c){.re = (a.re + b.re), .img = (a.img + b.img)});
}

static inline double	module_sq(t_c z)
{
	return (z.re * z.re + z.img * z.img);
}

static inline t_c	pow_i(t_c z, int power, int burn)
{
	t_c	z0;

	if (burn)
		z = (t_c){.re = custom_fabs(z.re), .img = custom_fabs(z.img)};
	z0 = z;
	while (power-- > 1)
		z = (t_c){.re = z.re * z0.re - z.img * z0.img, .img = z0.re * z.img + z0.img * z.re};
	return (z);
}

// static inline t_c     mandelbrot_op(t_c z, t_c c)
// {
//     return ((t_c){.re = z.re * z.re + z.img * z.img + c.re, .img = 2 * z.re * z.img + c.img});
// }

static inline t_c division_i(t_c a, t_c b)
{
    double	denom;

    denom = b.re * b.re + b.img * b.img;
    return ((t_c){.re = (a.re * b.re + a.img * b.img) / denom,
        .img = (a.img * b.re - a.re * b.img) / denom});
}

static inline t_c     multiplication(int a, t_c b)
{
    return ((t_c){.re = a * b.re, .img = a * b.img});
}

static  inline t_c     custom_op(t_c z, t_data img)
{
    return (sum_i(pow_i(z, img.power, img.burning_ship), (t_c){.re = img.c1, .img = img.c2}));
}

static  inline t_c     dcustom_op(t_c z, t_data img)
{
    return (multiplication(img.power, pow_i(z, img.power - 1, img.burning_ship)));
}

static inline int	custom_calc(t_c c, t_data img)
{
    t_c    next_c;
    t_c fz;
    t_c    dfz;
    t_c   fz2;
    int    i;

    next_c = (t_c){.re = c.re + 1, .img = c.img + 1};
    i = 0;
    while (i < img.max_iter)
    {
        fz = custom_op(next_c, img);
        dfz = dcustom_op(next_c, img);
        fz2 = division_i(fz, dfz);
        next_c = (t_c){.re = next_c.re - fz2.re, .img = next_c.img - fz2.img};
        if (module_sq(fz) < img.precision)
            break ;
        i++;
    }
    if (i == img.max_iter)
        return (0);
    if (module_sq(fz) < img.precision)
        return (i);
    return (i + img.logt[(int)(module_sq(fz) / img.precision)] / img.cos);
}


static inline t_c	mandelbrot_op(t_c z, t_c c, t_data img)
{
	return (sum_i(pow_i(z, img.power, img.burning_ship), c));
}

static int	mandelbrot_calc(t_c c, t_data img)
{
	t_c		z;
	int		i;
	double	mod_sq;

	z = (t_c){.re = 0, .img = 0};
	i = 0;
	mod_sq = 0;
	while (i < img.max_iter && mod_sq < 3)
	{
		z = mandelbrot_op(z, c, img);
		mod_sq = module_sq(z);
		i++;
	}
	if (i == img.max_iter)
		return (i);
	return (i);
}

int	mandelbrot(t_data img)
{
	double	x;
	double	y;
	int		m;

	x = 0;
	while (x <= img.width - 1)
	{
		y = 0;
		while (y <= img.height - 1)
		{
			m = custom_calc((t_c){.re = img.xmin + (x / img.width)
				* (img.xmax - img.xmin), .img = img.ymin + (y / img.height)
				* (img.ymax - img.ymin)}, img);
			my_pixel_put(&img, (int)x, (int)y, img.colorpalette[m]);
			y++;
		}
		x++;
	}
	return (0);
}

void save_image_to_bmp(t_data *img)
{
    char *filename;
    filename = arena_alloc(img->arena, 100 * sizeof(char));
    sprintf(filename, "img_%d-%d-%d-%f-%f-%d-%d.bmp", img->max_iter, img->power, img->colorint, img->c1, img->c2, img->burning_ship, img->current_fractal == &julia);
    printf("%s\n", filename);
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    int paddedRowSize = (img->width * 3 + 3) & (~3);
    int dataSize = paddedRowSize * img->height;

    // Bitmap file header
    unsigned char fileHeader[14] = {
        'B', 'M',           // Magic number for file
        0, 0, 0, 0,         // File size in bytes
        0, 0, 0, 0,         // Reserved
        54, 0, 0, 0         // Start of pixel array
    };
    // Update file size in header
    int fileSize = 54 + dataSize;
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    // Bitmap info header
    unsigned char infoHeader[40] = {
        40, 0, 0, 0,        // Info header size
        0, 0, 0, 0,         // Image width
        0, 0, 0, 0,         // Image height
        1, 0,               // Number of color planes
        24, 0,              // Bits per pixel
        0, 0, 0, 0,         // Compression
        0, 0, 0, 0,         // Image size
        0, 0, 0, 0,         // Horizontal resolution
        0, 0, 0, 0,         // Vertical resolution
        0, 0, 0, 0,         // Colors in color table
        0, 0, 0, 0          // Important color count
    };
    // Update image dimensions in header
    infoHeader[4] = img->width;
    infoHeader[5] = img->width >> 8;
    infoHeader[6] = img->width >> 16;
    infoHeader[7] = img->width >> 24;
    infoHeader[8] = img->height;
    infoHeader[9] = img->height >> 8;
    infoHeader[10] = img->height >> 16;
    infoHeader[11] = img->height >> 24;

    fwrite(fileHeader, 1, 14, file);
    fwrite(infoHeader, 1, 40, file);

    // Write pixel data
    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            // Get pixel data from mlx image (BGR format)
            int pixel = *(int *)(img->addr + (y * img->ll + x * (img->bpp / 8)));
            fwrite(&pixel, 1, 3, file); // Write 3 bytes (BGR)
        }
        // Padding for row
        unsigned char padding[3] = {0, 0, 0};
        fwrite(padding, 1, paddedRowSize - img->width * 3, file);
    }

    fclose(file);
}

void save_fdf(t_data img)
{
    char *filename;
    double x;
    double y;
    int pixel;
    
    filename = arena_alloc(img.arena, 100 * sizeof(char));
    sprintf(filename, "img_%d-%d-%f-%f.fdf", img.max_iter, img.power, img.c1, img.c2);
    printf("%s\n", filename);
    FILE *file = fopen(filename, "wb");
    if (!file) return;
	x = 0;
	while (x <= img.width - 1)
	{
		y = 0;
		while (y <= img.height - 1)
		{
            if (img.current_fractal == &mandelbrot)
                pixel = custom_calc((t_c){.re = img.xmin + (x / img.width)
                    * (img.xmax - img.xmin), .img = img.ymin + (y / img.height)
                    * (img.ymax - img.ymin)}, img);
            else if (img.current_fractal == &julia)
                pixel = julia_calc((t_c){.re = img.c1, .img = img.c2},
				(t_c){.re = img.xmin + (x / img.width) * (img.xmax - img.xmin),
				.img = img.ymin + (y / img.height) * (img.ymax - img.ymin)},
				img);
            else
                pixel = 0;
            // if (pixel >= img.max_iter - 10)
            //     fprintf(file, "%d ", (int)((x*x + y*y) / img.width / img.height));
            // else
                fprintf(file, "%d ", pixel * 1);
            y++;
        }
        fprintf(file, "\n");
        x++;
    }

    fclose(file);
}
