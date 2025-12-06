/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:28:30 by icseri            #+#    #+#             */
/*   Updated: 2025/12/06 14:42:14 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	uint32_t	pixel_index;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	pixel_index = y * WIDTH + x;
	((uint32_t *)data->img->pixels)[pixel_index] = color;
}

void	close_hook(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	safe_exit(&data->map, EXIT_SUCCESS);
}

void	loop_hook(void *param)
{
	t_data		*data;
	static int	prev_x = -1;
	int32_t		x;
	int32_t		y;
	int			delta_x;
	static double	last_update = 0;
	double		current_time;

	data = (t_data *)param;
	current_time = get_time();
	mlx_get_mouse_pos(data->mlx, &x, &y);
	if (prev_x == -1)
		prev_x = x;
	delta_x = x - prev_x;
	if (delta_x != 0)
	{
		prev_x = x;
		data->player_angle += delta_x * MOUSE_SENSITIVITY;
		if (data->player_angle < 0)
			data->player_angle += 2 * M_PI;
		if (data->player_angle >= 2 * M_PI)
			data->player_angle -= 2 * M_PI;
		render_scene(data);
	}
	else if (data->map.is_bonus && current_time - last_update >= FRAME_DURATION)
	{
		render_scene(data);
		last_update = current_time;
	}
}
