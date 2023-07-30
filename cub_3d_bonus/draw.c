/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhattaki <hhattaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:35:44 by hhattaki          #+#    #+#             */
/*   Updated: 2023/07/30 19:04:30 by hhattaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_square(int x, int y, t_mlx *m, int color)
{
	int	i;
	int	j;
	int	size;

	size = m->map.tile;
	i = x;
	while (i < size + x)
	{
		j = y;
		while (j < size + y)
		{
			// printf("%c\n", m->map.map[i / m->map.tile][j / m->map.tile]);
			my_mlx_pixel_put(m, i, j, color);
			j++;
		}
		i++;
	}
}

void	draw_player(double x, double y, t_mlx *t, int size)
{
	int	i;
	int	j;

	i = x - size;
	while (i < x + size)
	{
		j = y - size;
		while (j < y + size)
		{
			my_mlx_pixel_put(t, i, j, 0x55aa99FF);
			j++;
		}
		i++;
	}
}

void	draw_ray(t_mlx *m, double angle, double x, double y)
{
	int	size;
	int	l;
	int	x2;
	int	y2;

	size = 0;
	l = m->map.tile;
	x2 = x + cos(angle) * size;
	y2 = y + sin(angle) * size;
	while (size <= 25 && m->map.map[y2 / l][x2 / l] != '1' && m->map.map[y2
		/ l][x2 / l] != 'D')
	{
		my_mlx_pixel_put(m, x2, y2, 0x99aa99FF);
		size++;
		x2 = x + (cos(angle) * size);
		y2 = y + (sin(angle) * size);
	}
}

void	draw_map2(int j, t_mlx *m, int size)
{
	int	i;

	i = (m->p.x / m->map.tile) - 3;
	if (i < 0)
		i = 0;
	while (i < m->map.x_elements_nb - 1)
	{
		if (i > (m->p.x / m->map.tile) + 3 || j > (m->p.y / m->map.tile) + 3
			|| m->map.map[j][i] == 'X')
			draw_square(i * size, j * size, m, 0xFFFFFFFF);
		else
		{
			if (m->map.map[j][i] != '1' && m->map.map[j][i] != 'D')
				draw_square(i * size, j * size, m, 0x55FFFFFF);
			else
				draw_square(i * size, j * size, m, 0x55FF8FAB);
		}
		i++;
	}
}

void	my_mlx_pixel_put2(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length
		+ x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_map(t_mlx *m)
{
	int	j;
	int	i;
	int	size;

	size = m->map.tile;
	j = 0;
	while (j < (m->map.y_elements_nb - 1) * size)
	{
		i = 0;
		while (i < (m->map.x_elements_nb - 1) * size)
			my_mlx_pixel_put2(m, i++, j, 0xFFFFFFFF);
		j++;
	}
	j = (m->p.y / m->map.tile) - 3;
	if (j < 0)
		j = 0;
	while (j < m->map.y_elements_nb - 1)
	{
		draw_map2(j, m, size);
		j++;
	}
	draw_player(m->p.x, m->p.y, m, size / 15);
	draw_ray(m, m->p.angle, m->p.x, m->p.y);
}
