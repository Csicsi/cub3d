/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:36:26 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 11:09:59 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	update_animation_frame(t_data *data)
{
	double	current_time;

	current_time = get_time();
	if (current_time - data->last_frame >= FRAME_DURATION)
	{
		data->frame = (data->frame + 1) % NUM_FRAMES;
		data->last_frame = current_time;
	}
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	update_animation_frame(data);
	cast_rays(data);
	wall_height = 0;
	x = -1;
	while (++x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height);
		}
	}
	draw_minimap(data);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keydata.key == KEY_ESC && keydata.action == MLX_PRESS)
		mlx_close_window(data->mlx);
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		update_player_position(data, keydata.key);
		if (keydata.key == KEY_E)
			check_and_open_door_nearby(data, data->player_x + 0.5,
				data->player_y + 0.5);
	}
}
