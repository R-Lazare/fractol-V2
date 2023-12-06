/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 18:22:50 by rluiz             #+#    #+#             */
/*   Updated: 2023/07/15 19:24:54 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	*arena_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
		((unsigned char *)s)[i] = c;
	return (s);
}

static int	is_power_of_two(uintptr_t x)
{
	return ((x & (x - 1)) == 0);
}

static uintptr_t	align_forward(uintptr_t ptr, size_t align)
{
	uintptr_t	p;
	uintptr_t	a;
	uintptr_t	modulo;

	if (!is_power_of_two(align))
		exit(1);
	p = ptr;
	a = (uintptr_t)align;
	modulo = p % a;
	if (modulo != 0)
		p += a - modulo;
	return (p);
}

void	*arena_alloc(t_arena *a, size_t size)
{
	uintptr_t	curr_ptr;
	uintptr_t	offset;
	void		*ptr;

	curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
	offset = align_forward(curr_ptr, sizeof(void *));
	offset -= (uintptr_t)a->buf;
	if (offset + size > a->buf_size)
		return (NULL);
	ptr = &((unsigned char *)a->buf)[offset];
	a->prev_offset = offset;
	a->curr_offset = offset + size;
	arena_memset(ptr, 0, size);
	return (ptr);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 18:22:08 by rluiz             #+#    #+#             */
/*   Updated: 2023/07/15 20:14:02 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	*arena_init(size_t buffer_size)
{
	t_arena	*a;
	void	*buf;

	buf = malloc(buffer_size);
	if (!buf)
		return (NULL);
	a = malloc(sizeof(t_arena));
	if (!a)
		return (NULL);
	a->buf = buf;
	a->buf_size = buffer_size;
	a->curr_offset = 0;
	a->prev_offset = 0;
	return (a);
}

void	arena_reset(t_arena *a)
{
	a->curr_offset = 0;
	a->prev_offset = 0;
}

void	arena_destroy(t_arena *a)
{
	arena_reset(a);
	if (a->buf)
		free(a->buf);
	free(a);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 18:48:13 by rluiz             #+#    #+#             */
/*   Updated: 2023/07/18 19:48:41 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static double	*getlist_helper(double b, int n, t_data img)
{
	double	*rs;
	int		i;

	rs = arena_alloc(img.arena, n * sizeof(double));
	i = 0;
	while (i < n)
	{
		if (i == 0)
			rs[i] = 0.5 + b * 0.0001;
		else
			rs[i] = (3.98 + ((3.983 - 3.98) / n) * i ) * rs[i - 1] * (1 - rs[i - 1]);
		i++;
	}
	return (rs);
}

t_cls	*getlist(double b, t_data img, int n)
{
	int		j;
	t_cls	*list;
	t_cls	*next;
	double	*u;
	int		*x;

	j = 0;
	u = getlist_helper(b, n * 4, img);
	x = arena_alloc(img.arena, n * sizeof(int));
	while (j++ < n)
		x[j] = round(u[j % n] * 21 * img.colorint);
	j = 0;
	list = arena_alloc(img.arena, sizeof(t_cls));
	next = list;
	while (j < n)
	{
		next->next = arena_alloc(img.arena, sizeof(t_cls));
		next = next->next;
		next->r = x[j] * x[(j + 4) % n] % 256;
		next->g = x[(j + 1) % n] * x[(j + 4) % n] % 256;
		next->b = x[(j + 2) % n] * x[(j + 4) % n] % 256;
		j += 4;
	}
	return (list->next);
}

int	*colors_helper(int m, int max_iter, t_data img, t_cls *colorset)
{
	int	*color;
	int	i;
	int	c;
	t_cls	*next;

	if (img.colorint > max_iter / 2 - 1)
		img.colorint -= 1;
	c = img.colorint;
	i = 0;
	color = arena_alloc(img.arena, sizeof(int) * 3);
	next = colorset;
	while (i < c && next)
	{
		if (m < max_iter / c * (i + 1))
		{
			color[0] = next->r * (m % (max_iter / c)) / (max_iter / c);
			color[1] = next->g * (m % (max_iter / c)) / (max_iter / c);
			color[2] = next->b * (m % (max_iter / c)) / (max_iter / c);
			break ;
		}
		next = next->next;
		i++;
	}
	return (color);
}

int	*colors(int max_iter, t_data img)
{
	int	*color;
	int	i;
	int	*list;
	t_cls	*next;

	list = (int *)arena_alloc(img.arena, sizeof(int) * max_iter);
	i = 0;
	next = img.colorset;
	while (i < max_iter && next)
	{
		color = colors_helper(i, max_iter, img, next);
		list[i] = create_trgb(0, color[0], color[1], color[2]);
		i++;
		next = next->next;
	}
	return (list);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:08:21 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/05 19:38:41 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	main_mandelbrot(t_data img)
{
	img.zoom = 0;
	img.width = 1500;
	img.height = img.width * 0.75;
	img.xmin = -2;
	img.power = 2;
	img.max_iter = 20;
	img.xmax = 1;
	img.colorint = 1;
	img.ymin = -1;
	img.ymax = 1;
	img.x0 = (img.xmax - img.xmin) / 2;
	img.y0 = (img.ymax - img.ymin) / 2;
	img.colorset = getlist(img.colorint, img, img.colorint * 10000);
	img.colorpalette = colors(img.max_iter, img);
	img.cos = log(1.4 - (0.75 + cos(img.colorint * 0.1) / 3));
	img.modf = 0;
	img.burning_ship = 0;
	img.current_fractal = &mandelbrot;
	calc_log(&img);
	img.mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, img.width, img.height, "Mandelbrot");
	img.img = mlx_new_image(img.mlx, img.width, img.height);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.ll, &img.endian);
	img.current_fractal(img);
	mlx_mouse_hook(img.win, mouse_hook, &img);
	mlx_key_hook(img.win, key_hook, &img);
	mlx_hook(img.win, 17, 0, freeall, &img);
	mlx_put_image_to_window(img.mlx, img.win, img.img, 0, 0);
	mlx_loop(img.mlx);
	return (0);
}

int	main(void)
{
	t_data	img;

	img.arena = arena_init(2147483647);
	if (img.arena == NULL)
		return (0);
	main_mandelbrot(img);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:27:58 by rluiz             #+#    #+#             */
/*   Updated: 2023/07/15 18:16:09 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	ft_atod(char *str)
{
	double	nb;
	int		i;
	int		sign;
	int		j;

	nb = 0;
	i = 0;
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + (str[i++] - '0');
	if (str[i] == '.')
	{
		str++;
		j = 10;
		while (str[i] >= '0' && str[i] <= '9')
		{
			nb = nb + (double)(str[i] - '0') / j;
			str++;
			j *= 10;
		}
	}
	return (nb * sign);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != 0 && s2[i] != 0)
	{
		if (s1[i] != s2[i])
			return ((s1[i] - s2[i]));
		i++;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:42:00 by rluiz             #+#    #+#             */
/*   Updated: 2023/08/02 22:18:51 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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

int	julia_calc(t_c c, t_c z0, t_data img)
{
    t_c		z;
    int		i;
    double	mod_sq;

    z.re = z0.re;
    z.img = z0.img;
    i = 0;
    mod_sq = 0;
    while (i < img.max_iter && mod_sq < 4)
    {
        z = sum_i(pow_i(z, img.power, img.burning_ship), c);
        mod_sq = module_sq(z);
        i++;
    }
    if (i == img.max_iter)
    {
        return (i);
    }
    return (i + img.logt[(int)(mod_sq * 1000)] / img.cos);
}

int	julia(t_data img)
{
	double	x;
	double	y;
	int		m;
	t_c		c;
	t_c		z0;

	x = 0;
	c.re = img.c1;
	c.img = img.c2;
	while (x <= img.width)
	{
		y = 0;
		while (y <= img.height)
		{
			z0.re = img.xmin + (x / img.width) * (img.xmax - img.xmin);
			z0.img = img.ymin + (y / img.height) * (img.ymax - img.ymin);
			m = julia_calc(c, z0, img);
			my_pixel_put(&img, (int)x, (int)y, img.colorpalette[m]);
			y++;
		}
		x++;
	}
	return (0);
}/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:32:10 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/05 19:59:36 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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

static int	mandelbrot_calc(t_c c, t_data img)
{
	t_c		z;
	int		i;
	double	mod_sq;

	z.re = 0;
	z.img = 0;
	i = 0;
	mod_sq = 0;
	while (i < img.max_iter && mod_sq < 2.6 + img.modf * 0.05)
	{
		z = sum_i(pow_i(z, img.power, img.burning_ship), c);
		mod_sq = module_sq(z);
		i++;
	}
	if (i == img.max_iter)
	{
		return (i);
	}
	return (i + img.logt[(int)(mod_sq * 1000)] / img.cos);
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
			m = mandelbrot_calc((t_c){.re = img.xmin + (x / img.width)
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
    FILE *file = fopen("image.bmp", "wb");
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihelper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:32:00 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/05 18:25:02 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	my_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->ll + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	refresh_image(t_data *img)
{
	mlx_destroy_image(img->mlx, img->img);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->ll, &img->endian);
	img->current_fractal(*img);
	mlx_put_image_to_window(img->mlx, img->win, img->img, 0, 0);
	printf("zoom: %d, max_iter: %d, power: %d, colorint: %d, modf: %d, c1: %f, c2: %f, burn: %d, julia: %d\n", img->zoom, img->max_iter, img->power, img->colorint, img->modf, img->c1, img->c2, img->burning_ship, img->current_fractal == &julia);
}

int	key_hook_arrows(int keycode, t_data *img)
{
	if (keycode == 65362)
	{
		img->ymin = img->ymin - 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
		img->ymax = img->ymax - 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
	}
	if (keycode == 65364)
	{
		img->ymin = img->ymin + 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
		img->ymax = img->ymax + 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
	}
	if (keycode == 65361)
	{
		img->xmin = img->xmin - 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
		img->xmax = img->xmax - 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
	}
	if (keycode == 65363)
	{
		img->xmin = img->xmin + 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
		img->xmax = img->xmax + 0.1 * (1 / cosh(pow(img->zoom, 0.75)));
	}
	return (0);
}

void	expend_colorset(int n, t_data *img)
{
	t_cls	*next;
	int		i;

	next = img->colorset;
	while (next->next)
		next = next->next;
	next->next = getlist(img->colorint, *img, n);
	i = 0;
	next = img->colorset;
	while (next->next && i < n)
	{
		next = next->next;
		i++;
	}
	return ;
}

void	reboot(t_data *img)
{
	img->zoom = 0;
	img->width = 1500;
	if (img->current_fractal == &mandelbrot)
	{
		img->height = img->width * 0.75;
		img->xmin = -2;
		img->ymin = -1;
		img->ymax = 1;
	}
	else if (img->current_fractal == &julia)
	{
		img->height = img->width;
		img->xmin = -1;
		img->ymin = -1.2;
		img->ymax = 1.2;
	}
	img->xmax = 1;
	img->power = 2;
	img->max_iter = 20;
	img->modf = 0;
	img->x0 = (img->xmax - img->xmin) / 2;
	img->y0 = (img->ymax - img->ymin) / 2;
	img->colorset = getlist(img->colorint + 1, *img, img->colorint * 10000);
	img->colorint = img->colorint % 7;
	img->colorpalette = colors(img->max_iter, *img);
	log(custom_fabs(1.4 + img->zoom - (0.75 + cos(img->colorint * 0.1) * img->zoom / 3)));
}

int	key_hook(int keycode, t_data *img)
{
	if (keycode == 115)
		save_image_to_bmp(img);
	if (keycode == 98)
	{
		img->burning_ship = !img->burning_ship;
		img->modf = 40;
	}
	if (keycode == 120 && img->modf < 200)
		img->modf++;
	if (keycode == 119 && img->modf > -12)
		img->modf--;
	if (keycode == 106)
	{
		if (img->current_fractal == &mandelbrot)
		{
			img->current_fractal = &julia;
			img->c1 = 0;
			img->c2 = 0;
		}
		else if (img->current_fractal == &julia)
			img->current_fractal = &mandelbrot;
		reboot(img);
	}
	if (keycode == 65307)
		freeall(img);
	if (keycode == 65451)
		img->max_iter += 3;
	if (keycode == 65453)
	{
		img->max_iter -= 2;
		img->colorint--;
	}
	if (img->max_iter < 1)
		img->max_iter = 1;
	if (keycode == 114)
		reboot(img);
	if (keycode == 32)
	{
		img->colorint = img->colorint + 1 * (img->max_iter > img->colorint);
		expend_colorset(1, img);
	}
	if (keycode == 51 || keycode == 38)
		img->c2 += 0.04;
	if (keycode == 65289)
		img->c2 += 0.005;
	if (keycode == 52 || keycode == 233)
		img->c2 -= 0.04;
	if (keycode == 97)
		img->c2 -= 0.005;
	if (keycode == 49 || keycode == 34)
		img->c1 += 0.04;
	if (keycode == 122)
		img->c1 += 0.005;
	if (keycode == 50 || keycode == 39)
		img->c1 -= 0.04;
	if (keycode == 101)
		img->c1 -= 0.005;
	if (keycode == 112 && img->power < 9)
		img->power += 1;
	if (keycode == 111 && img->power > 1)
		img->power -= 1;
	else
		key_hook_arrows(keycode, img);
	img->colorpalette = colors(img->max_iter, *img);
	refresh_image(img);
	return (0);
}

int	mouse_hook(int button, int x, int y, t_data *img)
{
	double	zoomFactor;
	double	width;
	double	height;

	zoomFactor = 2.0;
	img->x0 = (double)x / img->width;
	img->y0 = (double)y / img->height;
	if (button == 4)
	{
		img->zoom++;
		width = (img->xmax - img->xmin) / zoomFactor;
		height = (img->ymax - img->ymin) / zoomFactor;
		img->xmin = img->xmin + img->x0 * width;
		img->xmax = img->xmin + width;
		img->ymin = img->ymin + img->y0 * height;
		img->ymax = img->ymin + height;
	}
	else if (button == 5 && img->zoom > 0)
	{
		img->zoom--;
		width = (img->xmax - img->xmin) * zoomFactor;
		height = (img->ymax - img->ymin) * zoomFactor;
		img->xmin = img->xmin - img->x0 * (width - (img->xmax - img->xmin));
		img->xmax = img->xmin + width;
		img->ymin = img->ymin - img->y0 * (height - (img->ymax - img->ymin));
		img->ymax = img->ymin + height;
	}
	refresh_image(img);
	return (0);
}

int	freeall(t_data *img)
{
	if (img->arena)
	{
		arena_destroy(img->arena);
		if (img->img)
		{
			mlx_destroy_image(img->mlx, img->img);
			if (img->win)
				mlx_destroy_window(img->mlx, img->win);
			if (img->mlx)
				mlx_destroy_display(img->mlx);
			if (img->mlx)
				free(img->mlx);
		}
	}
	exit(0);
	return (0);
}
