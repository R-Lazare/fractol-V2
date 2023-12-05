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
	img.width = 900;
	img.height = 600;
	img.xmin = -2;
	img.power = 2;
	img.max_iter = 20;
	img.xmax = 1;
	img.colorint = 1;
	img.ymin = -1;
	img.ymax = 1;
	img.x0 = (img.xmax - img.xmin) / 2;
	img.y0 = (img.ymax - img.ymin) / 2;
	img.colorset = getlist(img.colorint, img, img.colorint * 200);
	img.colorpalette = colors(img.max_iter, img);
	img.mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, img.width, img.height, "Mandelbrot");
	img.img = mlx_new_image(img.mlx, img.width, img.height);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.ll, &img.endian);
	img.current_fractal = &mandelbrot;
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
