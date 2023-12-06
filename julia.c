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
        z = (t_c){.re = z.re * z.re - z.img * z.img + c.re, .img = 2 * z.re * z.img + c.img};
        mod_sq = z.re * z.re + z.img * z.img;
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
}