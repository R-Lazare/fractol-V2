/* ************************************************************************** */
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

static inline t_c	pow_i(t_c z, int power)
{
	while (power-- > 1)
		z = (t_c){.re = z.re * z.re - z.img * z.img, .img = 2 * z.re * z.img};
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
	while (i < img.max_iter && mod_sq < 4)
	{
		z = sum_i(pow_i(z, img.power), c);
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

	img.cos = log(1.4 - (0.75 + cos(img.colorint * 0.1) / 3));
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
