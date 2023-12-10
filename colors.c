/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 18:48:13 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/10 18:13:29 by rluiz            ###   ########.fr       */
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
			rs[i] = 0.5 + b * 0.001;
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
