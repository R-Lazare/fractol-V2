/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihelper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:32:00 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/22 17:58:54 by rluiz            ###   ########.fr       */
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
}

int	key_hook_arrows(int keycode, t_data *img)
{
	if (keycode == 65362)
	{
		img->ymin = img->ymin - 0.1;
		img->ymax = img->ymax - 0.1;
	}
	if (keycode == 65364)
	{
		img->ymin = img->ymin + 0.1;
		img->ymax = img->ymax + 0.1;
	}
	if (keycode == 65361)
	{
		img->xmin = img->xmin - 0.1;
		img->xmax = img->xmax - 0.1;
	}
	if (keycode == 65363)
	{
		img->xmin = img->xmin + 0.1;
		img->xmax = img->xmax + 0.1;
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
	img->width = 800;
	if (img->current_fractal == &mandelbrot)
	{
		img->height = img->width * 1;
		img->xmin = -1.8;
		img->ymin = -1.8;
		img->ymax = 1.2;
	}
	else if (img->current_fractal == &julia)
	{
		img->height = img->width;
		img->xmin = -1;
		img->ymin = -1.2;
		img->ymax = 1.2;
	}
	img->xmax = 1.2;
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
	if (keycode == 65437)
		img->precision /= 10000;
	if (keycode == 65430)
		img->precision *= 1000;
	if (keycode == 104)
		printf("zoom: %d, max_iter: %d, power: %d, colorint: %d, modf: %d, c1: %f, c2: %f, burn: %d, julia: %d\n", img->zoom, img->max_iter, img->power, img->colorint, img->modf, img->c1, img->c2, img->burning_ship, img->current_fractal == &julia);
	if (keycode == 115)
	{
		save_fdf(*img);
		return (0);
	}
	if (keycode == 98)
	{
		img->burning_ship = !img->burning_ship;
		img->modf = 40;
	}
	if (keycode == 120 && img->modf < 200)
		img->modf++;
	if ((keycode == 119 || keycode == 120) && img->modf > -12)
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
	if (keycode == 118)
	{
		printf("1 r : %d, g : %d, b : %d\n", img->colorpalette[1] >> 16, (img->colorpalette[1] >> 8) & 0xFF, img->colorpalette[1] & 0xFF);
		printf("2 r : %d, g : %d, b : %d\n", img->colorpalette[2] >> 16, (img->colorpalette[2] >> 8) & 0xFF, img->colorpalette[2] & 0xFF);
		printf("3 r : %d, g : %d, b : %d\n", img->colorpalette[3] >> 16, (img->colorpalette[3] >> 8) & 0xFF, img->colorpalette[3] & 0xFF);
		printf("4 r : %d, g : %d, b : %d\n", img->colorpalette[4] >> 16, (img->colorpalette[4] >> 8) & 0xFF, img->colorpalette[4] & 0xFF);
		printf("5 r : %d, g : %d, b : %d\n", img->colorpalette[5] >> 16, (img->colorpalette[5] >> 8) & 0xFF, img->colorpalette[5] & 0xFF);
		printf("6 r : %d, g : %d, b : %d\n", img->colorpalette[6] >> 16, (img->colorpalette[6] >> 8) & 0xFF, img->colorpalette[6] & 0xFF);
		printf("7 r : %d, g : %d, b : %d\n", img->colorpalette[7] >> 16, (img->colorpalette[7] >> 8) & 0xFF, img->colorpalette[7] & 0xFF);
		printf("8 r : %d, g : %d, b : %d\n", img->colorpalette[8] >> 16, (img->colorpalette[8] >> 8) & 0xFF, img->colorpalette[8] & 0xFF);
		printf("9 r : %d, g : %d, b : %d\n", img->colorpalette[9] >> 16, (img->colorpalette[9] >> 8) & 0xFF, img->colorpalette[9] & 0xFF);
		printf("\n");
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
	if (keycode == 65289 || keycode == 119)
		img->c2 += 0.005;
	if (keycode == 52 || keycode == 233)
		img->c2 -= 0.04;
	if (keycode == 97 || keycode == 101)
		img->c2 -= 0.005;
	if (keycode == 49 || keycode == 34)
		img->c1 += 0.04;
	if (keycode == 122 || keycode == 65289)
		img->c1 += 0.005;
	if (keycode == 50 || keycode == 39)
		img->c1 -= 0.04;
	if (keycode == 113) //
		img->c1 -= 0.005;
	if (keycode == 112)// && img->power < 9)
		img->power += 1;
	if (keycode == 111)// && img->power > 1)
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
	else if (button == 5)// && img->zoom > 0)
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
