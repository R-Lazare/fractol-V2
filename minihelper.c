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
	if (keycode == 50 || keycode == 39)
		img->c1 -= 0.04;
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
	img->height = img->width * 0.75;
	img->xmin = -2;
	img->power = 2;
	img->max_iter = 20;
	img->xmax = 1;
	img->ymin = -1;
	img->ymax = 1;
	img->x0 = (img->xmax - img->xmin) / 2;
	img->y0 = (img->ymax - img->ymin) / 2;
	img->colorset = getlist(img->colorint + 1, *img, img->colorint * 10000);
	img->colorpalette = colors(img->max_iter, *img);
	img->cos = log(1.4 - (0.75 + cos(img->colorint * 0.1) / 3));
	refresh_image(img);
}

int	key_hook(int keycode, t_data *img)
{
	if (keycode == 65307)
		freeall(img);
	if (keycode == 65451)
		img->max_iter += 5;
	if (keycode == 65453)
		img->max_iter -= 5;
	if (img->max_iter < 1)
		img->max_iter = 1;
	if (keycode == 114)
		reboot(img);
	if (keycode == 32)
	{
		img->colorint = img->colorint + 1 * (img->max_iter > img->colorint);
		expend_colorset(50, img);
	}
	if (keycode == 51 || keycode == 38)
		img->c2 += 0.04;
	if (keycode == 52 || keycode == 233)
		img->c2 -= 0.04;
	if (keycode == 49 || keycode == 34)
		img->c1 += 0.04;
	if (keycode == 112)
		img->power += 1;
	if (keycode == 111)
		img->power -= 1;
	else
		key_hook_arrows(keycode, img);
	img->colorpalette = colors(img->max_iter, *img);
	refresh_image(img);
	return (0);
}

int	mouse_hook(int button, int x, int y, t_data *img)
{
	img->x0 = (double)x / img->width;
	img->y0 = (double)y / img->height;
	if (button == 4)
	{
		img->zoom++;
		img->xmin = img->xmin + (img->x0) * (img->xmax - img->xmin) / 2;
		img->xmax = img->xmax - (1 - img->x0) * (img->xmax - img->xmin) / 2;
		img->ymin = img->ymin + (img->y0) * (img->ymax - img->ymin) / 2;
		img->ymax = img->ymax - (1 - img->y0) * (img->ymax - img->ymin) / 2;
	}
	if (button == 5 && img->zoom > 0)
	{
		img->zoom--;
		img->xmin = img->xmin - (img->x0) * (img->xmax - img->xmin) / 2;
		img->xmax = img->xmax + (1 - img->x0) * (img->xmax - img->xmin) / 2;
		img->ymin = img->ymin - (img->y0) * (img->ymax - img->ymin) / 2;
		img->ymax = img->ymax + (1 - img->y0) * (img->ymax - img->ymin) / 2;
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
