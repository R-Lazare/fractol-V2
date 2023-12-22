/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:42:00 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/22 19:10:35 by rluiz            ###   ########.fr       */
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
		z = (t_c){.re = z.re * z0.re - z.img * z0.img, .img = z0.re * z.img
			+ z0.img * z.re};
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
	double x;
	double y;
	int m;

	x = 0;
	while (x <= img.width)
	{
		y = 0;
		while (y <= img.height)
		{
			m = julia_calc((t_c){.re = img.c1, .img = img.c2},
				(t_c){.re = img.xmin + (x / img.width) * (img.xmax - img.xmin),
				.img = img.ymin + (y / img.height) * (img.ymax - img.ymin)},
				img);
			my_pixel_put(&img, (int)x, (int)y, img.colorpalette[m]);
			y++;
		}
		x++;
	}
	return (0);
}