/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 20:48:13 by hhattaki          #+#    #+#             */
/*   Updated: 2023/07/28 17:33:14 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
			my_mlx_pixel_put(t, i, j, PURPLE);
			j++;
		}
		i++;
	}
}

void	renderer(void *t)
{
	t_mlx	*m;
	int		l;

	m = t;
	l = m->map.tile * SCALE_FACTOR;
	mlx_clear_window(m->mlx_ptr, m->win_ptr);
	m->map.map_img = mlx_new_image(m->mlx_ptr, l * m->map.x_elements_nb,
			l * m->map.y_elements_nb);
	m->addr = mlx_get_data_addr(m->map.map_img, &m->bits_per_pixel,
			&m->line_length, &m->endian);
	cast_rays(m);
	m->img_ptr = mlx_new_image(m->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	m->addr = mlx_get_data_addr(m->img_ptr, &m->bits_per_pixel,
			&m->line_length, &m->endian);
	draw_walls(m);
	mlx_put_image_to_window(m->mlx_ptr, m->win_ptr, m->img_ptr, 0, 0);
	mlx_destroy_image(m->mlx_ptr, m->img_ptr);
}

void	init(t_mlx	*m)
{
	m->key.w = 0;
	m->key.s = 0;
	m->key.a = 0;
	m->key.d = 0;
	m->key.left = 0;
	m->key.right = 0;
	if (calc_length_x(m->map.x_elements_nb)
		< calc_length_y(m->map.y_elements_nb))
		m->map.tile = calc_length_x(m->map.x_elements_nb);
	else
		m->map.tile = calc_length_y(m->map.y_elements_nb);
	m->p.x = 2 * m->map.tile + (m->map.tile / 2);
	m->p.y = 2 * m->map.tile + (m->map.tile / 2);
	m->p.radius = 5;
	m->p.turn = 0;
	m->p.turn = 1;
	m->p.speed = 5;
	m->p.rot_speed = 2 * (M_PI / 180);
	open_textures(m);
}

int	main(int argc, char **argv)
{
	t_mlx	*m;
	int		fd;
	int		start;

	if (argc == 2)
	{
		m = malloc(sizeof(t_mlx));
		init_pars(m);
		fd = open_map(argv[1]);
		start = textures(fd, m);
		height_len(fd, m);
		if (!m->t[0].path || !m->t[1].path || !m->t[2].path || !m->t[3].path
			|| m->map.sky_color == -1 || m->map.floor_color == -1)
			error_mess("identifier missing\n");
		map(argv[1], start, m);
		m->map.x_elements_nb = max_len(argv[1], start);
		printf("\npath->%s %s %s %s\ncolors->%d %d\nlen->%d height->%d\n",
			m->t[0].path, m->t[1].path, m->t[2].path, m->t[3].path,
			m->map.floor_color, m->map.sky_color, m->map.x_elements_nb,
			m->map.y_elements_nb);
		printf("%f\n", m->p.angle);
		m->rays = malloc(NB_RAYS * sizeof(t_ray));
		m->mlx_ptr = mlx_init();
		init(m);
		m->win_ptr = mlx_new_window(m->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "cub3d");
		mlx_hook(m->win_ptr, 17, 0, red_cross, 0);
		mlx_hook(m->win_ptr, 2, 0, keys_down, m);
		mlx_loop_hook(m->mlx_ptr, move, m);
		mlx_hook(m->win_ptr, 3, 0, keys_up, m);
		mlx_loop(m->mlx_ptr);
	}
	else
		error_mess("no\n");
}
