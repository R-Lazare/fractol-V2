/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:37:11 by rluiz             #+#    #+#             */
/*   Updated: 2023/12/06 16:00:42 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H

# define FRACTOL_H
# include "minilibx-linux/mlx.h"
# include <math.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_arena
{
	void	*buf;
	size_t	buf_size;
	size_t	prev_offset;
	size_t	curr_offset;
}			t_arena;

typedef struct s_cls
{
	int		r;
	int		g;
	int		b;
	void	*next;
}			t_cls;

typedef struct s_data
{
	void	*img;
	int		width;
	int		(*current_fractal)(struct s_data);
	int		max_iter;
	int		height;
	void	*arena;
	int		*colorpalette;
	int		burning_ship;
	int		zoom;
	char	*addr;
	t_cls	*colorset;
	int		bpp;
	int		ll;
	int		endian;
	void	*mlx;
	double	xmin;
	int		power;
	double	c1;
	double	c2;
	double	xmax;
	int		colorint;
	double	ymin;
	double	ymax;
	double	x0;
	double	y0;
	int		modf;
	void	*win;
	double	cos;
	double	*logt;
}			t_data;

typedef struct nb_c
{
	double	re;
	double	img;
}			t_c;

double		custom_fabs(double x);
int			facto(int n);
void		save_image_to_bmp(t_data *img);
int			julia(t_data img);
void		calc_log(t_data *img);
void		expend_colorset(int n, t_data *img);
void		my_pixel_put(t_data *data, int x, int y, int color);
int			create_trgb(int t, int r, int g, int b);
int			key_hook(int keycode, t_data *img);
int			mandelbrot(t_data img);
int			ft_strcmp(const char *s1, const char *s2);
double		ft_atod(char *str);
int			mouse_hook(int button, int x, int y, t_data *img);
int			*colors(int max_iter, t_data img);
t_cls		*getlist(double b, t_data img, int n);
int			key_hook_arrows(int keycode, t_data *img);
void		*arena_init(size_t buffer_size);
void		arena_reset(t_arena *a);
void		*arena_alloc(t_arena *a, size_t size);
void		arena_destroy(t_arena *a);
int			freeall(t_data *img);
#endif