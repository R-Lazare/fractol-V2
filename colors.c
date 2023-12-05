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
	double	*u;
	double	*rs;
	double	*x;
	int		i;

	rs = arena_alloc(img.arena, n * sizeof(double));
	x = arena_alloc(img.arena, n * sizeof(double));
	u = arena_alloc(img.arena, n * sizeof(double));
	i = 0;
	while (i < n)
	{
		rs[i] = 3.98 + ((3.985 - 3.98) / n) * i;
		i++;
	}
	x[0] = 0.5 + b * 0.0001;
	i = 0;
	while (i++ < n)
		x[i] = rs[i] * x[i - 1] * (1 - x[i - 1]);
	i = 0;
	while (i < (int)(n * 0.95))
	{
		u[i] = x[i + (int)(0.95 * n)];
		i++;
	}
	return (u);
}

int	**getlist(double b, t_data img, int n)
{
	int		j;
	int		**list;
	double	*u;
	int		*x;

	j = 0;
	list = arena_alloc(img.arena, n * sizeof(int *));
	u = getlist_helper(b, n, img);
	x = arena_alloc(img.arena, n * sizeof(int));
	n = n - (int)(0.95 * n);
	while (j++ < n)
		x[j] = round(u[j % n] * 255);
	j = 0;
	while (j < n)
	{
		list[j / 4] = arena_alloc(img.arena, 3 * sizeof(int));
		list[j / 4][0] = x[j] * x[(j + 4) % n] % 256;
		list[j / 4][1] = x[(j + 1) % n] * x[(j + 4) % n] % 256;
		list[j / 4][2] = x[(j + 2) % n] * x[(j + 4) % n] % 256;
		j += 4;
	}
	return (list);
}

int	*colors_helper(int m, int max_iter, t_data img)
{
	int	*color;
	int	i;
	int	c;

	if (img.colorint > max_iter / 2 - 1)
		img.colorint -= 1;
	c = img.colorint;
	i = 0;
	color = arena_alloc(img.arena, sizeof(int) * 3);
	while (i < c)
	{
		if (m < max_iter / c * (i + 1))
		{
			color[0] = img.colorset[i][0] * (m % (max_iter / c)) / (max_iter
					/ c);
			color[1] = img.colorset[i][1] * (m % (max_iter / c)) / (max_iter
					/ c);
			color[2] = img.colorset[i][2] * (m % (max_iter / c)) / (max_iter
					/ c);
			break ;
		}
		i++;
	}
	return (color);
}

int	*colors(int max_iter, t_data img)
{
	int	*color;
	int	i;
	int	*list;

	list = (int *)arena_alloc(img.arena, sizeof(int) * max_iter);
	i = 0;
	while (i < max_iter)
	{
		color = colors_helper(i, max_iter, img);
		list[i] = create_trgb(0, color[0], color[1], color[2]);
		i++;
	}
	return (list);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}
