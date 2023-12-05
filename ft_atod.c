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
